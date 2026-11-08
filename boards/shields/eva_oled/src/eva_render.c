/*
 * EVA OLED renderer - port of the approved HTML prototype (eva-oled-proposta.html).
 * SPDX-License-Identifier: MIT
 */
#include "eva_render.h"
#include "eva_assets.h"

#include <string.h>

/* ---------- small math (no libm needed) ---------- */
#define EVA_PI 3.14159265f

static float fsin(float x) {
    /* range-reduce to [-pi, pi] then 7th-order polynomial; error < 1e-4 */
    const float tp = 2.0f * EVA_PI;
    int k = (int)(x / tp);
    x -= (float)k * tp;
    if (x > EVA_PI) x -= tp;
    if (x < -EVA_PI) x += tp;
    /* fold to [-pi/2, pi/2] */
    if (x > EVA_PI / 2) x = EVA_PI - x;
    if (x < -EVA_PI / 2) x = -EVA_PI - x;
    float x2 = x * x;
    return x * (1.0f - x2 / 6.0f * (1.0f - x2 / 20.0f * (1.0f - x2 / 42.0f)));
}
static float fcos(float x) { return fsin(x + EVA_PI / 2); }
static int jround(float v) { /* JS Math.round: round half up */
    float f = v + 0.5f;
    int i = (int)f;
    if ((float)i > f) i--; /* floor for negatives */
    return i;
}
static int iceil(float v) {
    int i = (int)v;
    if ((float)i < v) i++;
    return i;
}

/* ---------- framebuffer primitives ---------- */
static eva_fb_t *fb;

static void P(int x, int y, int v) {
    if (x >= 0 && x < EVA_W && y >= 0 && y < EVA_H) fb->px[y][x] = (uint8_t)v;
}
static int G(int x, int y) {
    return (x >= 0 && x < EVA_W && y >= 0 && y < EVA_H) ? fb->px[y][x] : 0;
}
static void X(int x, int y) {
    if (x >= 0 && x < EVA_W && y >= 0 && y < EVA_H) fb->px[y][x] ^= 1;
}
static void fill(int x, int y, int w, int h, int v) {
    for (int j = y; j < y + h; j++)
        for (int i = x; i < x + w; i++) P(i, j, v);
}
static void rect(int x, int y, int w, int h, int v) {
    for (int i = x; i < x + w; i++) { P(i, y, v); P(i, y + h - 1, v); }
    for (int j = y; j < y + h; j++) { P(x, j, v); P(x + w - 1, j, v); }
}
static void dots(int y) {
    for (int i = 0; i < EVA_W; i += 2) P(i, y, 1);
}
static void stripes(int x, int y, int w, int h, int t, int inv) {
    for (int j = y; j < y + h; j++)
        for (int i = x; i < x + w; i++) P(i, j, (((i + j + t) >> 2) & 1) ^ inv);
}

static int glyph_index(char c) {
    if (c >= 'a' && c <= 'z') c = (char)(c - 32);
    if (c < 32 || c > 95) c = ' ';
    return c - 32;
}

/* 3x5 font */
static int t3(const char *s, int x, int y, int v, int sc) {
    for (; *s; s++) {
        const uint8_t *g = EVA_F3[glyph_index(*s)];
        for (int r = 0; r < 5; r++)
            for (int k = 0; k < 3; k++)
                if (g[r] & (4 >> k)) fill(x + k * sc, y + r * sc, sc, sc, v);
        x += 4 * sc;
    }
    return x;
}
static int w3(const char *s, int sc) { return (int)strlen(s) * 4 * sc - sc; }

/* 5x7 font */
static void t5(const char *s, int x, int y, int v) {
    for (; *s; s++) {
        const uint8_t *g = EVA_F5[glyph_index(*s)];
        for (int r = 0; r < 7; r++)
            for (int k = 0; k < 5; k++)
                if (g[r] & (16 >> k)) P(x + k, y + r, v);
        x += 6;
    }
}
static int w5(const char *s) { return (int)strlen(s) * 6 - 1; }

/* 5x7 font rotated 90 deg (reads top-to-bottom), scaled */
static void t5rot(const char *s, int x, int y, int v, int sc) {
    for (; *s; s++) {
        const uint8_t *g = EVA_F5[glyph_index(*s)];
        for (int r = 0; r < 7; r++)
            for (int k = 0; k < 5; k++)
                if (g[r] & (16 >> k)) fill(x + (6 - r) * sc, y + k * sc, sc, sc, v);
        y += 6 * sc;
    }
}

static void itoa_u(unsigned v, char *buf) {
    char tmp[8];
    int n = 0;
    do { tmp[n++] = (char)('0' + v % 10); v /= 10; } while (v && n < 7);
    for (int i = 0; i < n; i++) buf[i] = tmp[n - 1 - i];
    buf[n] = 0;
}

/* scrolling ticker: one 3x5 char every 4px, scrolled by `off` pixels */
static void ticker(const char *text, int y, uint32_t t, uint32_t ms_per_px, int chars) {
    int len = (int)strlen(text);
    int off = (int)((t / ms_per_px) % (uint32_t)(len * 4));
    for (int k = 0; k < chars; k++) {
        char c[2] = {text[(off / 4 + k) % len], 0};
        t3(c, k * 4 - (off % 4), y, 1, 1);
    }
}

/* ---------- NERV leaf ---------- */
static int leaf_bit(int y, int x) {
    if (y < 0 || y >= EVA_LEAF_H || x < 0 || x >= EVA_LEAF_W) return 0;
    return (EVA_LEAF[y] >> (EVA_LEAF_W - 1 - x)) & 1;
}
static void leaf_half(int ox, int oy) { /* 2x2 OR downsample */
    for (int y = 0; y < EVA_LEAF_H; y += 2)
        for (int x = 0; x < EVA_LEAF_W; x += 2)
            if (leaf_bit(y, x) | leaf_bit(y, x + 1) | leaf_bit(y + 1, x) | leaf_bit(y + 1, x + 1))
                P(ox + x / 2, oy + y / 2, 1);
}
static void serif(const uint8_t *g, int w, int x, int y) {
    for (int j = 0; j < 9; j++)
        for (int i = 0; i < w; i++)
            if (g[j] & (1 << (w - 1 - i))) X(x + i, y + j);
}
static void nerv_full(int ox, int oy) {
    for (int y = 0; y <= 7; y++) { /* diagonal stroke */
        int x = jround((float)ox + 3.0f + 12.0f * (float)y / 7.0f);
        P(x, oy + y, 1);
        P(x + 1, oy + y, 1);
    }
    for (int y = 0; y < EVA_LEAF22_H; y++)
        for (int x = 0; x < EVA_LEAF22_W; x++)
            if (EVA_LEAF22[y] & (1u << (EVA_LEAF22_W - 1 - x))) P(ox + 10 + x, oy + y, 1);
    int x = ox + 1;
    serif(EVA_SERIF_N, EVA_SERIF_N_W, x, oy + 8); x += EVA_SERIF_N_W + 1;
    serif(EVA_SERIF_E, EVA_SERIF_E_W, x, oy + 8);
    x = ox + 11;
    serif(EVA_SERIF_R, EVA_SERIF_R_W, x, oy + 18); x += EVA_SERIF_R_W + 1;
    serif(EVA_SERIF_V, EVA_SERIF_V_W, x, oy + 18);
    for (int d = 15; d <= 165; d++) { /* arc under the letters */
        float r = (float)d * EVA_PI / 180.0f;
        int ax = jround((float)ox + 15.0f + 15.0f * fcos(r));
        int ay = jround((float)oy + 20.0f + 12.0f * fsin(r));
        if (!G(ax, ay)) P(ax, ay, 1);
    }
}

/* ---------- AT field hexagon ring ---------- */
static void hex_ring(int cx, int cy, float r, float age, int clip_top) {
    if (((int)(age * 10.0f)) % 2 && age > 0.6f) return;
    for (int a = 0; a < 6; a++) {
        float a1 = EVA_PI / 3.0f * (float)a + EVA_PI / 6.0f, a2 = a1 + EVA_PI / 3.0f;
        float x1 = (float)cx + r * fcos(a1), y1 = (float)cy + r * fsin(a1);
        float x2 = (float)cx + r * fcos(a2), y2 = (float)cy + r * fsin(a2);
        int n = iceil(r) + 2;
        for (int k = 0; k <= n; k++) {
            int x = jround(x1 + (x2 - x1) * (float)k / (float)n);
            int y = jround(y1 + (y2 - y1) * (float)k / (float)n);
            if (y >= clip_top) P(x, y, 1);
        }
    }
}

/* ---------- EMERGENCY honeycomb ---------- */
static uint32_t hash32(int32_t in) {
    uint32_t n = (uint32_t)in;
    n = (n ^ 61u) ^ (n >> 16);
    n = n + (n << 3);
    n ^= n >> 4;
    n = n * 0x27d4eb2du;
    return n ^ (n >> 15);
}
static void em_on(int x, int y, int v, int y0, int y1) {
    if (y >= y0 && y < y1) P(x, y, v);
}
static void em_cell(int cx, int cy, int y0, int y1) {
    static const char *up[3] = {"..#..", ".###.", "#####"};
    static const char *dn[3] = {"#####", ".###.", "..#.."};
    for (int j = 0; j < 13; j++) {
        int e = j < 12 - j ? j : 12 - j;
        int hw = 3 + jround((float)e * 4.0f / 6.0f);
        for (int x = cx - hw; x <= cx + hw; x++) em_on(x, cy + j, 1, y0, y1);
    }
    for (int j = 0; j < 3; j++)
        for (int i = 0; i < 5; i++)
            if (up[j][i] == '#') em_on(cx - 2 + i, cy + 2 + j, 0, y0, y1);
    for (int x = cx - 5; x <= cx + 5; x++)
        if (x != cx - 2 && x != cx + 2) em_on(x, cy + 6, 0, y0, y1);
    for (int j = 0; j < 3; j++)
        for (int i = 0; i < 5; i++)
            if (dn[j][i] == '#') em_on(cx - 2 + i, cy + 9 + j, 0, y0, y1);
}

static void layer_footer(const char *n, int y_bar, int y_txt) {
    if (eva_layer_is_game(n)) {
        fill(0, y_bar, EVA_W, 9, 1);
        t3(n, (EVA_W - w3(n, 1)) >> 1, y_txt, 0, 1);
    } else {
        t3(n, (EVA_W - w3(n, 1)) >> 1, y_txt, 1, 1);
    }
}

static void draw_emergency(const struct eva_left_state *s, uint32_t t) {
    const int y0 = 14, y1 = 111;
    ticker("EMERGENCY  ", y0 + 1, t, 60, 10);
    static const int cxs[2] = {8, 24};
    for (int c = 0; c < 2; c++) {
        for (int r = -1; r < 7; r++) {
            int cy = y0 + 9 + r * 15 + (c ? 8 : 0);
            uint32_t h = hash32(c * 31 + r * 7 + 5);
            uint32_t per = 380 + (h % 900);
            uint32_t ph = (h >> 10) % per;
            uint32_t duty = 50 + ((h >> 20) % 35); /* percent */
            if (((t + ph) % per) * 100 < per * duty) em_cell(cxs[c], cy, y0 + 8, y1);
        }
    }
    dots(113);
    layer_footer(s->layer_name, 116, 118);
}

bool eva_layer_is_game(const char *name) { return name && name[0] == 'G'; }

/* ---------- left (central) screen ---------- */
void eva_render_left(eva_fb_t *f, const struct eva_left_state *s, uint32_t t) {
    fb = f;
    memset(fb, 0, sizeof(*fb));
    char buf[12];
    const char *layer = (s->layer_name && s->layer_name[0]) ? s->layer_name : "BASE";
    char lname[6];
    strncpy(lname, layer, 5);
    lname[5] = 0;
    struct eva_left_state st = *s;
    st.layer_name = lname;

    /* output: USB / BTn (blinks while the BT profile is not connected) */
    if (s->output_usb) {
        t3("USB", 0, 0, 1, 1);
    } else {
        buf[0] = 'B'; buf[1] = 'T'; buf[2] = (char)('1' + s->ble_profile); buf[3] = 0;
        if (s->ble_connected || ((t >> 9) & 1)) t3(buf, 0, 0, 1, 1);
    }
    itoa_u(s->battery, buf);
    strcat(buf, "%");
    t3(buf, EVA_W - w3(buf, 1), 0, 1, 1);

    bool low = s->battery < 15 && !s->usb_powered;
    if (s->usb_powered) {
        t3("EXT PWR", 2, 7, 1, 1);
    } else {
        t3("INT", 0, 7, 1, 1);
        int on = (s->battery + 19) / 20;
        for (int i = 0; i < 5; i++) {
            int x = 13 + i * 4;
            if (i < on && !(low && ((t >> 9) & 1))) fill(x, 7, 3, 5, 1);
            else rect(x, 7, 3, 5, 1);
        }
    }
    dots(14);
    if (low) {
        draw_emergency(&st, t);
        return;
    }

    /* layer panel */
    const int py = 17, ph = 34;
    if (eva_layer_is_game(lname)) {
        stripes(0, py, EVA_W, ph, (int)(t >> 5), 0);
        fill(2, py + 9, 28, 16, 0);
        if ((t >> 8) & 1) rect(2, py + 9, 28, 16, 1);
        t5(lname, (EVA_W - w5(lname)) / 2, py + 13, 1);
    } else {
        rect(0, py, EVA_W, ph, 1);
        t3("LAYER", 2, py + 3, 1, 1);
        t5(lname, (EVA_W - w5(lname)) / 2, py + 15, 1);
        for (int i = 0; i < 3; i++) {
            int x = 9 + i * 5;
            if (i == s->layer_index) fill(x, py + 27, 3, 3, 1);
            else rect(x, py + 27, 3, 3, 1);
        }
    }
    dots(54);

    /* SYNC rate (WPM) */
    t3("SYNC", 0, 58, 1, 1);
    itoa_u(s->wpm, buf);
    t3(buf, EVA_W - w3(buf, 2), 58, 1, 2);
    t3("WPM", 0, 64, 1, 1);
    const int gy = 82;
    float amp = 2.0f + (float)s->wpm / 12.0f;
    if (amp > 10.0f) amp = 10.0f;
    for (int i = 0; i < EVA_W; i += 4) P(i, gy, 1);
    int prev = -1000;
    for (int x = 0; x < EVA_W; x++) {
        float ft = (float)t;
        int y = jround((float)gy + fsin(((float)x + ft / 40.0f) * 0.55f) * amp *
                                       fsin(((float)x + ft / 90.0f) * 0.18f));
        if (prev != -1000) {
            int a = prev < y ? prev : y, b = prev < y ? y : prev;
            for (int j = a; j <= b; j++) P(x, j, 1);
        } else {
            P(x, y, 1);
        }
        prev = y;
    }
    dots(96);

    if (s->caps) {
        fill(0, 99, EVA_W, 9, 1);
        t3("CAPS", 8, 101, 0, 1);
    } else {
        t3("EVA-00", (EVA_W - w3("EVA-00", 1)) >> 1, 101, 1, 1);
    }
    leaf_half(0, 109);
    t3("NERV", 16, 116, 1, 1);
}

/* ---------- right (peripheral) screen ---------- */
static const char *TAG = "  GOD'S IN HIS HEAVEN. ALL'S RIGHT WITH THE WORLD.   ";

static void warn(uint32_t t, const char *reason) {
    stripes(0, 14, 6, EVA_H - 23, (int)(t >> 5), 0);
    stripes(EVA_W - 6, 14, 6, EVA_H - 23, (int)(t >> 5), 0);
    int blink = (t >> 9) & 1;
    if (blink) fill(7, 14, 18, EVA_H - 23, 1);
    t5rot("WARNING", 8, 30, blink ? 0 : 1, 2);
    t3(reason, (EVA_W - w3(reason, 1)) >> 1, EVA_H - 6, 1, 1);
}

void eva_render_right(eva_fb_t *f, const struct eva_right_state *s, uint32_t t) {
    fb = f;
    memset(fb, 0, sizeof(*fb));
    char buf[8];
    t3(s->linked ? "LNK" : "---", 0, 0, 1, 1);
    itoa_u(s->battery, buf);
    strcat(buf, "%");
    t3(buf, EVA_W - w3(buf, 1), 0, 1, 1);
    int on = (s->battery + 19) / 20;
    for (int i = 0; i < 5; i++) {
        int x = 1 + i * 6;
        if (i < on) fill(x, 7, 5, 3, 1);
        else rect(x, 7, 5, 3, 1);
    }
    if (!s->linked) { warn(t, "NO LINK"); return; }
    if (s->battery < 15 && !s->usb_powered) { warn(t, "BAT LOW"); return; }

    nerv_full(0, 14);
    ticker(TAG, 52, t, 70, 12);
    dots(60);
    t3("AT FIELD", 0, 64, 1, 1);

    const int cx = 16, cy = 96;
    bool any = false;
    for (int i = 0; i < EVA_RIPPLES; i++) {
        uint32_t r0 = s->ripples[i];
        if (!r0 || t < r0) continue;
        float age = (float)(t - r0) / 1000.0f;
        if (age > 1.2f) continue;
        any = true;
        hex_ring(cx, cy, 3.0f + age * 24.0f, age, 71);
    }
    if (!any) hex_ring(cx, cy, 4.0f + fsin((float)t / 400.0f), 0.0f, 71);
}
