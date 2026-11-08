/* Host preview: renders key states of the EVA screens to PBM files (not part of the firmware). */
#include <stdio.h>
#include <string.h>
#include "../src/eva_render.h"

static void save(const char *name, const eva_fb_t *fb) {
    char path[256];
    snprintf(path, sizeof path, "%s.pbm", name);
    FILE *f = fopen(path, "wb");
    fprintf(f, "P1\n%d %d\n", EVA_W, EVA_H);
    for (int y = 0; y < EVA_H; y++) {
        for (int x = 0; x < EVA_W; x++) fputc(fb->px[y][x] ? '1' : '0', f);
        fputc('\n', f);
    }
    fclose(f);
}

int main(void) {
    static eva_fb_t fb;
    struct eva_left_state L = {.battery = 82, .output_usb = false, .ble_profile = 0, .ble_connected = true,
                               .layer_name = "BASE", .layer_index = 0, .caps = false, .wpm = 64};
    eva_render_left(&fb, &L, 1000); save("L_base", &fb);
    L.layer_name = "LOWER"; L.layer_index = 1; L.caps = true;
    eva_render_left(&fb, &L, 1300); save("L_lower_caps", &fb);
    L.layer_name = "GAME"; L.layer_index = 3; L.caps = false;
    eva_render_left(&fb, &L, 1000); save("L_game", &fb);
    L.layer_name = "G-NUM"; L.output_usb = true; L.usb_powered = true;
    eva_render_left(&fb, &L, 1700); save("L_gnum_usb", &fb);
    L.layer_name = "GAME"; L.output_usb = false; L.usb_powered = false; L.battery = 9;
    eva_render_left(&fb, &L, 2300); save("L_emergency", &fb);

    struct eva_right_state R = {.battery = 76, .linked = true};
    eva_render_right(&fb, &R, 5000); save("R_nerv_idle", &fb);
    R.ripples[0] = 4700; R.ripples[1] = 4950;
    eva_render_right(&fb, &R, 5000); save("R_nerv_ripples", &fb);
    R.battery = 9; memset(R.ripples, 0, sizeof R.ripples);
    eva_render_right(&fb, &R, 5600); save("R_warn_bat", &fb);
    R.battery = 70; R.linked = false;
    eva_render_right(&fb, &R, 5000); save("R_warn_link", &fb);
    puts("ok");
    return 0;
}
