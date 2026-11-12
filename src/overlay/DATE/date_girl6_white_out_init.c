/* @SymbolStart: 0x80157274 */
/* @SymbolSize: 268 */
/* @CompileEnv: IDO52_CONST_INREG_VALUES=127 */
/* @PostProc: swap_instructions --offset1 0x54 --offset2 0x60 */
/* @PostProc: swap_instructions --offset1 0x5c --offset2 0x60 */
/* @PostProc: swap_instructions --offset1 0x7c --offset2 0x80 */
/* @PostProc: swap_instructions --offset1 0xec --offset2 0xf0 */
/* @PostProc: patch_insn_reg_field 0x94:rd:t4:t9 0x98:rt:t4:t9 0xa8:rt:t9:t4 0xb0:rd:t8:t5 0xb0:rs:t9:t4 0xb8:rt:t8:t5 */
/* Function: date_girl6_white_out_init (true entry 0x80157274) */
/* Overlay: DATE */
/* cracked by `@CompileEnv: IDO52_CONST_INREG_VALUES=127` (Hook 16 IS */
#include "types.h"
#include "tokimeki/class_w.h"

extern s32 sub_sub_step_up();

struct Blk { s32 w[17]; };

extern struct Blk cw96_active;
extern struct Blk cw98_active;
extern s16 cw98_anim;
extern u8 Sprites[];
extern volatile u8 cal_cw_disp;
extern volatile u8 cw97_disp;
extern s32 fade_flag;
extern u8 back_clear_r;
extern u8 back_clear_g;
extern u8 back_clear_b;
extern CLASS_W class_w[];

s32 date_girl6_white_out_init(void)
{
    s16 v1;
    s32 *p;

    cw98_active = cw96_active;
    cw98_anim = 4;
    class_w[98].anim_flag = 9;

    v1 = 0;
    do {
        p = (s32 *)(Sprites + v1 * 36);
        v1 = (s16)(v1 + 1);
        p[36] = p[36] | 0x80000000;
    } while (v1 < 6);

    cal_cw_disp = cal_cw_disp & 0xFF7F;
    cw97_disp = cw97_disp & 0xFF7F;
    fade_flag = 0;
    back_clear_g = 127;
    back_clear_r = 127;
    back_clear_b = 127;
    return sub_sub_step_up();
}
