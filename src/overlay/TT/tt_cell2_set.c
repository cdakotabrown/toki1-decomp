/* @PostProc: patch_insn_reg_field 0x18:rt:a1:a0 0x20:rt:a0:t7 0x24:rt:a1:a0 0x28:rd:a0:t7 0x28:rs:a0:t7 0x2c:rs:a0:t7 0x2c:rt:a0:t7 0x30:rt:t7:t8 0x34:rt:t8:t9 0x38:rt:t9:t0 0x3c:rt:t0:t1 0x40:rt:t2:t3 0x44:rd:t1:t2 0x48:rd:t2:t3 0x48:rs:t2:t3 0x48:rt:t1:t2 0x4c:rt:t7:t8 0x50:rt:t8:t9 0x54:rt:a1:a0 0x58:rt:t9:t0 0x5c:rt:t0:t1 0x60:rt:a0:t7 0x64:rs:t2:t3 0x64:rt:t2:t3 0x6c:rt:t2:t3 */
/* tt_cell2_set - TT overlay, Address: 0x8013AB50, Size: 96 bytes (24 insns) */
#include "types.h"

extern u8 *tt_boss_wk;
extern u8 tt_cell2_frame_tbl[];
extern s16 tt_cell2_ofs_tbl[];

void tt_cell2_set(void)
{
    u8 *p;
    u16 i;
    u8 t;
    s32 k;

    p = tt_boss_wk;
    i = *(u16 *)(p + 936);
    k = 2;
    p[160] = k;
    t = tt_cell2_frame_tbl[i];
    *(s16 *)(p + 594) = 26;
    *(s16 *)(p + 642) = 36;
    p[180] = k;
    p[182] = 14;
    *(s16 *)(p + 596) = 38;
    p[162] = t;
    *(s16 *)(p + 644) = tt_cell2_ofs_tbl[i];
}
