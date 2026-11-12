/* @SymbolStart: 0x80138548 */
/* @SymbolSize: 164 */
/* @CompileEnv: IDO52_VETO_FORMLIV=4 IDO52_PRECOLOR_WEB=v0:0 */
/* @PostProc: swap_instructions --offset1 0x84 --offset2 0x88 */
/* @PostProc: swap_instructions --offset1 0x90 --offset2 0x94 */
/* @PostProc: patch_insn_reg_field 0x10:rt:a1:v0 0x14:rs:a1:v0 0x14:rt:a1:v0 0x1c:rs:a1:v0 0x40:rt:a1:v0 0x44:rs:a1:v0 0x44:rt:a1:v0 0x4c:rs:a1:v0 0x4c:rt:v1:v0 0x50:rt:v1:v0 0x60:rt:t9:t8 0x64:rs:t9:t8 0x64:rt:t9:t8 0x70:rd:t0:t9 0x70:rs:t9:t8 0x74:rs:t0:t9 0x7c:rt:t1:t2 0x80:rs:t1:t2 0x80:rt:t1:t2 0x84:rt:v1:v0 0x8c:rs:t1:t2 0x8c:rt:t2:t3 0x90:rd:t3:t0 0x90:rt:v1:v0 0x94:rt:t2:t3 0x98:rt:t3:t0 */
/* shugaku_cal_cw_scroll - SHUGAKU overlay, Address: 0x80138548 (absorbed head), */
#include "types.h"

extern void k_reset(s32 a0);
extern void class_clear_n(s32 a0);
extern void sub_sub_step_up(void);

extern s16 cal_cw_x;
extern s16 cal_cw_tick;
extern s32 pad_data;

s32 shugaku_cal_cw_scroll(void)
{
    s32 v0;
    s32 t7;

    v0 = (s32)cal_cw_x;
    if (v0 < -479) {
        k_reset(1);
        class_clear_n(96);
        sub_sub_step_up();
    }
    v0 = (s32)cal_cw_x;
    v0 = v0 - 3;
    t7 = (s32)(s16)v0;
    cal_cw_x = (s16)t7;
    v0 = t7;
    if ((pad_data & 0x00600060) != 0) {
        cal_cw_tick = (s16)(cal_cw_tick + 1);
        v0 = t7 - 3;
        v0 = (s32)(s16)v0;
        cal_cw_x = (s16)v0;
    }
    return v0;
}
