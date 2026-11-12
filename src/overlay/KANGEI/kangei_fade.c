/* @CompileFlags: -cckr */
/* @SymbolStart: 0x80136630 */
/* @SymbolSize: 488 */
/* Address: 0x80136630 (absorbed cross-prologue head; the .sym draws the start */
/* at the prologue 0x80136638), Size: 488 bytes (122 insns) incl. the head */
#include "types.h"

extern u8 kangei_fade_g;
extern u8 kangei_fade_b;
extern u8 kangei_fade_r;
extern s32 step_counter;
extern u8 fade_bri;

void sprite_rgb_set();
void k_reset();
void prim_init();
void sub_sub_step_up();
void kangei_sprite_disp();
void dtd_on_tpage();

kangei_fade()
{
    s32 i;

    if (step_counter == 0) {
        kangei_fade_g = 0;
        kangei_fade_b = 0;
        kangei_fade_r = 0;
        step_counter = 1;
    } else if (step_counter == 1) {
        kangei_fade_r += 16;
        kangei_fade_g += 16;
        kangei_fade_b += 16;
        if (kangei_fade_r >= 240) {
            step_counter = 2;
            kangei_fade_g = 255;
            kangei_fade_b = 255;
            kangei_fade_r = 255;
            for (i = 58; i < 64; i = (s16)(i + 1)) {
                sprite_rgb_set((s16)i, 0);
            }
            fade_bri = 0;
        }
    } else {
        kangei_fade_r -= 8;
        kangei_fade_g -= 8;
        kangei_fade_b -= 8;
        if (kangei_fade_r < 9) {
            kangei_fade_g = 0;
            kangei_fade_b = 0;
            kangei_fade_r = 0;
            k_reset(1);
            prim_init();
            sub_sub_step_up();
        }
    }
    kangei_sprite_disp(-160, -120, 320, 160, kangei_fade_r, kangei_fade_g, kangei_fade_b,
                  9, 1);
    dtd_on_tpage(0, 0, 9, 1, 0);
}
