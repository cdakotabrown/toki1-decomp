/* @SymbolStart: 0x8013D100 */
/* @SymbolSize: 200 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:t6 0x14:rs:v0:t6 0x14:rt:v0:t6 0x18:rt:a0:v0 0x1c:rd:t6:t7 0x1c:rt:v0:t6 0x20:rs:t6:t7 0x20:rt:v0:t6 0x24:rd:t8:t7 0x28:rd:a0:v0 0x28:rs:a0:v0 0x28:rt:t8:t7 0x2c:rs:a0:v0 0x2c:rt:a0:v0 0x30:rt:v1:t8 0x34:rs:v1:t8 0x34:rt:v1:t8 0x3c:rs:a0:v0 0x40:rs:v1:t8 0x48:rt:t2:t0 0x4c:rs:t2:t0 0x4c:rt:t2:t0 0x50:rd:t0:t1 0x50:rt:a0:v0 0x54:rd:t1:t2 0x54:rt:t0:t1 0x58:rs:t1:t0 */
/* Function: date_birth_day_check */
/* Overlay: DATE */
/* Address: 0x8013D128 (entry 0x8013D100 — 10-word cross-prologue head) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void goto_step(s32);
extern void goto_sub_step(s32);

extern u8 girl_no;
extern u8 girl0_birth[];
extern volatile u8 now_month;
extern u8 now_day;
extern s32 p_birthday;

void date_birth_day_check(void)
{
    s32 w;
    s32 rec;
    s32 sel;

    w = girl_no;
    rec = *(s32 *)(girl0_birth + (((w << 3) - w) << 3));
    sel = *(u8 *)&now_month;

    if (sel == (rec & 0xF) &&
        now_day == (s32)(((u32)rec << 23) >> 27)) {
        goto_step(80);
        goto_sub_step(2);
        return;
    }

    rec = p_birthday;
    if (now_month == (rec & 0xF) &&
        now_day == (s32)(((u32)rec << 23) >> 27)) {
        goto_step(80);
        goto_sub_step(3);
        return;
    }

    sub_sub_step_up();
}
