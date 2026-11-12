/* @SymbolSize: 164 */
/* @PostProc: scheduler_normalizer */
/* @PostProc: patch_insn_reg_field 0x14:rt:t8:t7 0x28:rs:t8:t7 0x28:rt:t8:t7 0x2c:rd:t7:t6 0x38:rs:t7:t6 0x38:rt:t8:t7 0x64:rt:t9:t8 0x6c:rs:t9:t8 */
/* Address: 0x80151190 */
/* Size: 164 bytes (41 insns) */
#include "types.h"

extern s32 taco_row_ang_tbl[];

extern s32 taco_row_ang_tbl_0_6;
extern s32 taco_row_ang_tbl_1_6;

extern void angle_set(s32 a0, s32 a1);

void taco_angle_row_set(s32 base, volatile s32 sel)
{
    s32 i;
    s32 a0;
    s32 *row;

    i = 0;
    a0 = base;
    row = &taco_row_ang_tbl[*(s32 *)&sel];
    do {
        angle_set(a0, *row);
        i++;
        a0++;
        row++;
    } while (i != 6);

    if (sel != 0) {
        angle_set(27, taco_row_ang_tbl_1_6);
    } else {
        angle_set(26, taco_row_ang_tbl_0_6);
    }
}
