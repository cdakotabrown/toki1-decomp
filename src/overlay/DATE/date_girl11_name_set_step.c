/* @SymbolStart: 0x8015A520 */
/* @SymbolSize: 52 */
/* @PostProc: patch_insn_reg_field 0x10:rt:t6:a0 0x14:rs:t6:a0 0x14:rt:t6:a0 0x28:rt:t6:a0 */
/* Function: date_girl11_name_set_step */
/* Overlay: DATE */
/* Address: 0x8015A520 (cross-prologue head) / 0x8015A528 (body) */
/* Size: 52 bytes (13 instructions) */
#include "types.h"

extern u8 girl_no;
extern u8 now_girl;

extern void sub_sub_step_up(void);
extern void date_name_set(void);

void date_girl11_name_set_step(void)
{
    now_girl = girl_no;
    date_name_set();
    sub_sub_step_up();
}
