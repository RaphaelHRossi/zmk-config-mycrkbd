/*
 * EVA OLED - ZMK status screen glue (ZMK v0.3 / LVGL 8).
 * Polls keyboard state on an LVGL timer (display thread), renders a vertical
 * 32x128 frame with eva_render.c and blits it rotated onto the 128x32 SSD1306.
 * SPDX-License-Identifier: MIT
 */
#include <string.h>
#include <zephyr/kernel.h>
#include <lvgl.h>

#include <zmk/display/status_screen.h>
#include <zmk/event_manager.h>
#include <zmk/events/position_state_changed.h>

#if IS_ENABLED(CONFIG_ZMK_BATTERY_REPORTING)
#include <zmk/battery.h>
#endif
#if IS_ENABLED(CONFIG_USB_DEVICE_STACK)
#include <zmk/usb.h>
#endif

#define EVA_IS_CENTRAL (!IS_ENABLED(CONFIG_ZMK_SPLIT) || IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL))

#if EVA_IS_CENTRAL
#include <zmk/endpoints.h>
#include <zmk/keymap.h>
#if IS_ENABLED(CONFIG_ZMK_BLE)
#include <zmk/ble.h>
#endif
#if IS_ENABLED(CONFIG_ZMK_WPM)
#include <zmk/wpm.h>
#endif
#if IS_ENABLED(CONFIG_ZMK_HID_INDICATORS)
#include <zmk/hid_indicators.h>
#endif
#else
#if IS_ENABLED(CONFIG_ZMK_SPLIT_BLE)
#include <zmk/split/bluetooth/peripheral.h>
#endif
#endif

#include "eva_render.h"

#define DISP_W 128
#define DISP_H 32

static lv_color_t cbuf[DISP_W * DISP_H];
static lv_obj_t *canvas;
static eva_fb_t frame, last_frame;
static bool have_last;

/* ---- key presses -> AT Field ripples (any thread) ---- */
static uint32_t ripples[EVA_RIPPLES];
static atomic_t ripple_idx;

static int eva_key_listener(const zmk_event_t *eh) {
    const struct zmk_position_state_changed *ev = as_zmk_position_state_changed(eh);
    if (ev != NULL && ev->state) {
        uint32_t now = k_uptime_get_32();
        ripples[(uint32_t)atomic_inc(&ripple_idx) % EVA_RIPPLES] = now ? now : 1;
    }
    return ZMK_EV_EVENT_BUBBLE;
}
ZMK_LISTENER(eva_oled_keys, eva_key_listener);
ZMK_SUBSCRIPTION(eva_oled_keys, zmk_position_state_changed);

/* ---- common state ---- */
static uint8_t battery_level(void) {
#if IS_ENABLED(CONFIG_ZMK_BATTERY_REPORTING)
    return zmk_battery_state_of_charge();
#else
    return 100;
#endif
}
static bool usb_powered(void) {
#if IS_ENABLED(CONFIG_USB_DEVICE_STACK)
    return zmk_usb_is_powered();
#else
    return false;
#endif
}

#if EVA_IS_CENTRAL
static const char *const fallback_names[] = {"BASE", "LOWER", "RAISE", "GAME", "G-NUM"};
static char layer_buf[8];
static uint32_t wpm_fx; /* smoothed WPM, 8.8 fixed point */

static void fill_left(struct eva_left_state *s) {
    s->battery = battery_level();
    s->usb_powered = usb_powered();

    struct zmk_endpoint_instance ep = zmk_endpoints_selected();
    s->output_usb = ep.transport == ZMK_TRANSPORT_USB;
#if IS_ENABLED(CONFIG_ZMK_BLE)
    s->ble_profile = (uint8_t)zmk_ble_active_profile_index();
    s->ble_connected = zmk_ble_active_profile_is_connected();
#else
    s->ble_profile = 0;
    s->ble_connected = false;
#endif

    zmk_keymap_layer_index_t idx = zmk_keymap_highest_layer_active();
    const char *name = zmk_keymap_layer_name(zmk_keymap_layer_index_to_id(idx));
    if (name == NULL || name[0] == '\0') {
        name = idx < ARRAY_SIZE(fallback_names) ? fallback_names[idx] : "L?";
    }
    size_t i = 0;
    for (; name[i] && i < sizeof(layer_buf) - 1; i++) {
        char c = name[i];
        layer_buf[i] = (c >= 'a' && c <= 'z') ? (char)(c - 32) : c;
    }
    layer_buf[i] = '\0';
    s->layer_name = layer_buf;
    s->layer_index = idx;

#if IS_ENABLED(CONFIG_ZMK_HID_INDICATORS)
    s->caps = (zmk_hid_indicators_get_current_profile() & BIT(1)) != 0;
#else
    s->caps = false;
#endif

#if IS_ENABLED(CONFIG_ZMK_WPM)
    uint32_t target = (uint32_t)zmk_wpm_get_state() << 8;
    wpm_fx = (wpm_fx * 3 + target) / 4; /* ease towards ZMK's WPM */
#endif
    uint32_t w = (wpm_fx + 128) >> 8;
    s->wpm = (uint16_t)(w > 999 ? 999 : w);
}
#else
static void fill_right(struct eva_right_state *s) {
    s->battery = battery_level();
    s->usb_powered = usb_powered();
#if IS_ENABLED(CONFIG_ZMK_SPLIT_BLE)
    s->linked = zmk_split_bt_peripheral_is_connected();
#else
    s->linked = true;
#endif
    memcpy(s->ripples, ripples, sizeof(ripples));
}
#endif

/* ---- blit: vertical 32x128 frame -> 128x32 display buffer ---- */
static inline lv_color_t px_color(bool on) {
    /* corne.dtsi uses `inversion-on`, so black = lit (same convention as nice_oled) */
    if (IS_ENABLED(CONFIG_EVA_OLED_INVERT)) on = !on;
    return on ? lv_color_black() : lv_color_white();
}

static void blit_and_invalidate(void) {
    int minx = DISP_W, maxx = -1;
    for (int y = 0; y < EVA_H; y++) {
        for (int x = 0; x < EVA_W; x++) {
            uint8_t v = frame.px[y][x];
            if (have_last && last_frame.px[y][x] == v) continue;
            int dx, dy;
            if (IS_ENABLED(CONFIG_EVA_OLED_FLIP)) {
                dx = y;
                dy = (DISP_H - 1) - x;
            } else {
                dx = (DISP_W - 1) - y;
                dy = x;
            }
            cbuf[dy * DISP_W + dx] = px_color(v);
            if (dx < minx) minx = dx;
            if (dx > maxx) maxx = dx;
        }
    }
    memcpy(&last_frame, &frame, sizeof(frame));
    if (!have_last) {
        have_last = true;
        lv_obj_invalidate(canvas);
        return;
    }
    if (maxx < 0) return; /* nothing changed: no I2C traffic */
    lv_area_t a;
    lv_obj_get_coords(canvas, &a);
    lv_coord_t x0 = a.x1;
    a.x1 = x0 + minx;
    a.x2 = x0 + maxx;
    lv_obj_invalidate_area(canvas, &a);
}

static void eva_tick(lv_timer_t *timer) {
    ARG_UNUSED(timer);
    uint32_t t = k_uptime_get_32();
#if EVA_IS_CENTRAL
    struct eva_left_state s = {0};
    fill_left(&s);
    eva_render_left(&frame, &s, t);
#else
    struct eva_right_state s = {0};
    fill_right(&s);
    eva_render_right(&frame, &s, t);
#endif
    blit_and_invalidate();
}

lv_obj_t *zmk_display_status_screen(void) {
    lv_obj_t *screen = lv_obj_create(NULL);
    lv_obj_set_style_pad_all(screen, 0, 0);
    lv_obj_set_style_border_width(screen, 0, 0);
    lv_obj_clear_flag(screen, LV_OBJ_FLAG_SCROLLABLE);

    for (size_t i = 0; i < ARRAY_SIZE(cbuf); i++) cbuf[i] = px_color(false);
    canvas = lv_canvas_create(screen);
    lv_canvas_set_buffer(canvas, cbuf, DISP_W, DISP_H, LV_IMG_CF_TRUE_COLOR);
    lv_obj_align(canvas, LV_ALIGN_TOP_LEFT, 0, 0);

    eva_tick(NULL);
    lv_timer_create(eva_tick, CONFIG_EVA_OLED_FRAME_MS, NULL);
    return screen;
}
