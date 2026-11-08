/*
 * EVA OLED renderer - pure C, no ZMK/LVGL dependencies (host-testable).
 * Draws a vertical 32x128 1-bit frame; the ZMK glue rotates it onto the 128x32 SSD1306.
 * SPDX-License-Identifier: MIT
 */
#pragma once
#include <stdbool.h>
#include <stdint.h>

#define EVA_W 32
#define EVA_H 128
#define EVA_RIPPLES 8

typedef struct {
    uint8_t px[EVA_H][EVA_W]; /* 0 = off, 1 = lit */
} eva_fb_t;

struct eva_left_state {
    uint8_t battery;         /* 0..100 */
    bool usb_powered;        /* cable connected (EXT PWR, no EMERGENCY) */
    bool output_usb;         /* HID output goes to USB */
    uint8_t ble_profile;     /* 0-based */
    bool ble_connected;      /* active BLE profile connected */
    const char *layer_name;  /* upper-case, up to 5 chars shown */
    uint8_t layer_index;     /* 0..n */
    bool caps;
    uint16_t wpm;            /* already smoothed */
};

struct eva_right_state {
    uint8_t battery;
    bool usb_powered;
    bool linked;                       /* connected to the central half */
    uint32_t ripples[EVA_RIPPLES];     /* key-press timestamps (ms), 0 = empty */
};

void eva_render_left(eva_fb_t *fb, const struct eva_left_state *s, uint32_t t_ms);
void eva_render_right(eva_fb_t *fb, const struct eva_right_state *s, uint32_t t_ms);
bool eva_layer_is_game(const char *name);
