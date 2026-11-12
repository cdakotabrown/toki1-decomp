/* Status: WIP, Address: 0x8013FB60, Size: 128 bytes (32 insns) */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: swap_instructions --offset1 0x38 --offset2 0x3c */
#include "types.h"

extern s32 get_g_zyotai_h(s32 a0);
extern s32 bustup_timer_clamp(s32 a0);
extern void sub_sub_step_up(void);

extern u8 update_anim_state_tbl4;
extern u8 class_w[];

void geko_particle_disp_off(void) {
    s16 i;
    s32 mask;
    s32 stride;

    bustup_timer_clamp(get_g_zyotai_h(10));
    update_anim_state_tbl4 = 15;

    i = 0;
    mask = -129;
    stride = 68;
    do {
        class_w[(s32)i * stride + 6735] &= mask;
        i = (s16)(i + 1);
    } while (i < 10);
    sub_sub_step_up();
}
