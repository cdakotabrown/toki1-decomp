/* @SymbolStart: 0x80149B8C */
/* @SymbolSize: 132 */
/* @PostProc: patch_insn_reg_field 0x18:rt:t6:t7 0x1c:rs:t6:t7 0x1c:rt:t6:t7 0x24:rt:t7:t6 0x30:rt:t8:t0 0x34:rs:t8:t0 0x34:rt:t8:t0 0x38:rd:t9:t8 0x3c:rd:t0:t9 0x3c:rt:t9:t8 0x40:rs:t8:t9 0x44:rt:v1:v0 0x48:rs:v1:v0 0x48:rt:v1:v0 0x50:rs:v1:v0 0x58:rs:v1:v0 */
/* Function: date_birthday_check */
/* Overlay: DATE */
/* Address: 0x80149B9C */
/* Size: 116 bytes (29 instructions) */
#include "types.h"

extern s32 sub_sub_step_up();
extern void goto_step(s32);
extern void goto_sub_step(s32);

extern u32 p_birthday;
extern u8 now_month;
extern u8 now_day;
extern u8 rec_idx;

void date_birthday_check(void)
{
    u32 v0;
    u32 w;

    w = p_birthday;
    if ((w & 0xF) != now_month) {
        goto L_fail;
    }
    if ((s32)((w << 23) >> 27) != now_day) {
        goto L_fail;
    }

    v0 = rec_idx;
    if (v0 < 254) {
        rec_idx = (u8)(v0 - 1);
    }
    goto_step(80);
    goto_sub_step(3);
    return;

L_fail:
    sub_sub_step_up();
}
