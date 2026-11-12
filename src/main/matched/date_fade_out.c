/* Debug Symbol: date_fade_out (from o.bin, high match) */
/* Address: 0x8008560C - 0x800856B0 (164 bytes, 41 instructions) */
/* Region: IDO (>= 0x80081000) */
/* Compiler: IDO 5.2 via ido-static-recomp */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack 32 48 */
/* @PostProc: swap_branch_operands --offsets 0x28 */
/* @PostProc: scheduler_normalizer */
#include "types.h"

extern u8 now_sub_sub_step;
extern u8 fade_bri;
extern u8 cal_cw_bright;
extern u8 cw97_bright;
extern s32 date_sprite_on;

extern void fade_out_step(void);
extern void date_sprite_switch(s32 arg0);
extern void mes_window_switch(s32 arg0);
extern void sprite_rgb_set(s16 arg0, u8 arg1);

void date_fade_out(void) {
    u8 old_state;
    s16 i;
    u8 color;

    old_state = now_sub_sub_step;

    fade_out_step();

    if (now_sub_sub_step == old_state) {
        goto after_update;
    }
    date_sprite_switch(0);
    date_sprite_on = 0;
    mes_window_switch(1);

after_update:
    color = fade_bri;
    cal_cw_bright = color;
    cw97_bright = color;

    for (i = 58; i < 64; i++) {
        sprite_rgb_set(i, fade_bri);
    }
}
