/* @SymbolStart: 0x8013EF30 */
/* @SymbolSize: 116 */
/* Address: 0x8013EF30 (cross-prologue head absorbed), Size: 116 bytes */
/* @PostProc: patch_insn_reg_field 0x14:rt:a3:t6 0x20:rs:a3:t6 0x20:rt:t0:t7 0x24:rt:t7:t3 0x28:rs:t0:t7 0x28:rt:v1:t8 0x2c:rs:v1:t8 0x2c:rt:a2:t0 0x30:rd:t8:t2 0x30:rt:a2:t0 0x34:rd:t8:t2 0x34:rs:t8:t2 0x34:rt:a2:t0 0x38:rd:t8:t2 0x38:rt:t8:t2 0x3c:rd:t8:t2 0x3c:rs:t8:t2 0x3c:rt:a2:t0 0x40:rs:a3:t6 0x40:rt:v1:t8 0x44:rd:t8:t2 0x44:rt:t8:t2 0x48:rs:a3:t6 0x48:rt:a2:t0 0x4c:rs:t7:t3 0x4c:rt:t7:t3 0x50:rd:t8:t2 0x50:rs:t8:t2 0x50:rt:a2:t0 0x54:rd:t8:t2 0x54:rt:t8:t2 0x58:rd:v0:v1 0x58:rs:t7:t2 0x58:rt:t8:t3 0x5c:rs:v0:v1 0x5c:rt:t2:t4 0x64:rs:v0:v1 0x70:rs:v0:v1 0x70:rt:t2:t4 */
#include "types.h"

extern s32 ClearOTagR();
extern u8 *tt_db_buf;
extern s32 *tt_disp_wk;

void tt_dbuf_swap(void)
{
    s32 *p = tt_disp_wk;
    s32 t7, t8, t0, idx;
    s32 *v1;
    s32 t4;

    t7 = p[2];
    t8 = t7 + 1;
    t0 = t8 & 0x1;
    idx = t0 * 32952;
    *(volatile s32 *)&p[2] = t8;
    *(volatile s32 *)&p[2] = t0;
    v1 = (s32 *)(tt_db_buf + idx);
    t4 = v1[0];
    v1[1] = t4;
    ClearOTagR(v1 + 0x2002, 16);
}
