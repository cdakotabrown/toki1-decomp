/* @PostProc: patch_insn_reg_field 0x18:rt:a0:t7 0x24:rd:a0:t7 0x24:rs:a0:t7 0x28:rs:a0:t7 0x28:rt:a0:t7 0x2c:rt:t7:t8 0x30:rt:t8:t9 0x34:rt:t9:t0 0x38:rt:t0:t1 0x3c:rt:t2:t3 0x40:rd:t1:t2 0x44:rd:t2:t3 0x44:rs:t2:t3 0x44:rt:t1:t2 0x48:rt:t7:t8 0x4c:rt:t8:t9 0x54:rt:t9:t0 0x58:rt:t0:t1 0x5c:rt:a0:t7 0x60:rs:t2:t3 0x60:rt:t2:t3 0x68:rt:t2:t3 */
/* tt_cell3_set - TT overlay, Address: 0x8013ABB0, Size: 92 bytes (23 insns) */
#include "types.h"

extern u8 *tt_boss_wk;
extern u8 tt_cell3_frame_tbl[];
extern s16 tt_cell3_ofs_tbl[];

void tt_cell3_set(void)
{
    u8 *p;
    u16 i;
    u8 t;

    p = tt_boss_wk;
    i = *(u16 *)(p + 938);
    p[200] = 0;
    t = tt_cell3_frame_tbl[i];
    *(s16 *)(p + 598) = -26;
    *(s16 *)(p + 646) = 36;
    p[220] = 0;
    p[222] = 14;
    *(s16 *)(p + 600) = -38;
    p[202] = t;
    *(s16 *)(p + 648) = tt_cell3_ofs_tbl[i];
}
