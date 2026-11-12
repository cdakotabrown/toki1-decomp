/* @SymbolSize: 360 */
/* @CompileEnv: IDO52_COPYPROP_SITE=lod:12:deny,lod:13:deny,lod:20:deny IDO52_FORCE_WEB_COLOR=2:s1,33:v1,45:v0 IDO52_UGEN_TEMP_REG=5:t3,6:t4,7:t5,8:t6,10:t4,11:t3,12:t8,14:t5,15:t6,17:t5,18:t1,19:t8,21:t6,22:t2,26:t8,27:t9 */
/* @PostProc: swap_instructions --offset1 0x108 --offset2 0x10c */
/* @PostProc: swap_instructions --offset1 0x118 --offset2 0x11c */
/* ending_fade_step - ENDING overlay, Address: 0x801375BC, Size: 360 bytes */
/* ending_serifu_reset.c already ABSORBED (`@SymbolStart: 0x80137724`, */
/* `@SymbolSize: 52`, byte-perfect 13/13).  They were NOT alignment */
/* Fixed with `@SymbolSize: 360`; ending_serifu_reset still resolves 13/13. */
#include "types.h"

extern void k_reset(s32 a0);
extern void sprite_rgb_set(s16 a0, s32 a1);
extern void grad_box_disp(s32 x, s32 y, s32 w, s32 h, s32 n, u32 rgba0,
                          u32 rgba1);
extern void dtd_on(s32 a0);
extern void sub_sub_step_up(void);

extern u8 bustup2_cw_bright;
extern volatile u8 bustup2_cw_bright_v;
extern volatile u8 bustup_cw_bright;
extern u8 class_w[];

void ending_fade_step(void)
{
    s32 s1;
    s16 s0;
    u32 r0;
    u32 b;
    u32 r1;
    u32 g1;
    u32 rgba0;
    u32 rgba1;

    k_reset(0);

    s1 = bustup2_cw_bright;
    s1 = s1 - 1;
    bustup2_cw_bright = s1;
    if (bustup2_cw_bright < 8U) {
        bustup2_cw_bright_v = 0;
        sub_sub_step_up();
    }

    s1 = bustup2_cw_bright_v;
    bustup_cw_bright = s1;

    for (s0 = 4; s0 < 19; s0++) {
        s1 = bustup2_cw_bright_v; sprite_rgb_set(s0, s1);
    }

    for (s0 = 0; s0 < 14; s0++) {
        class_w[s0 * 68 + 6535] = bustup2_cw_bright_v;
    }

    s1 = bustup2_cw_bright_v;
    r0 = ((u32)(s1 * 192) >> 7) & 0xFF;
    b = ((u32)(s1 * 255) >> 7) & 0xFF;
    rgba0 = (r0 << 16) | (r0 << 8) | b;
    r1 = ((u32)(s1 * 64) >> 7) & 0xFF;
    g1 = ((u32)(s1 * 128) >> 7) & 0xFF;
    rgba1 = (r1 << 16) | (g1 << 8) | b;

    grad_box_disp(-160, -120, 320, 240, 14, rgba0, rgba1);
    dtd_on(14);
}
