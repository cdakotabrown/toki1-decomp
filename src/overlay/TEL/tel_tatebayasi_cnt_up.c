/* @SymbolStart: 0x8013A69C */
/* @SymbolSize: 48 */
/* Address: 0x8013A69C (2-insn cross-prologue head) / 0x8013A6A4 (body) */
/* Size: 48 bytes (12 insns, head absorbed) */
#include "types.h"

extern u8 girl11_tel_cnt;
extern void sub_sub_step_up(void);

void tel_tatebayasi_cnt_up(void)
{
    girl11_tel_cnt = girl11_tel_cnt + 1;
    sub_sub_step_up();
}
