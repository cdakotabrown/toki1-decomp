/* date_param_set -- DATE overlay, Address: 0x8013A2D8 */
/* @SymbolStart: 0x8013A2C4 */
/* @SymbolSize: 140 */
/* @PostProc: patch_insn_reg_field 0x10:rt:t6:a0 0x14:rs:t6:a0 0x20:rd:a1:t6 0x28:rd:a1:t6 0x28:rs:a1:t6 0x30:rs:a1:t6 0x60:rs:a1:t6 */
#include "types.h"

extern u8   now_girl;
extern u8  *date_impkao_tbl_4_imp;
extern u8  *date_impkao_tbl_4_kao;
extern s16 kao[];
extern s16 imp[];

extern s32 date_trans_set();

s32 date_param_set(void)
{
    u32 state;
    u8 *p;

    state = now_girl;
    p = date_impkao_tbl_4_imp + state * 3;
    imp[0] = p[0];
    imp[1] = p[1];
    imp[2] = p[2];
    p = date_impkao_tbl_4_kao + state * 3;
    kao[0] = p[0];
    kao[1] = p[1];
    kao[2] = p[2];
    return date_trans_set(state);
}
