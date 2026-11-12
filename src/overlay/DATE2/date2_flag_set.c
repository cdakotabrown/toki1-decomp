/* @SymbolStart: 0x80133714 */
/* date2_flag_set - DATE2 overlay, Size: 92 bytes (23 insns) */
/* @SymbolSize: 100 */
/* @PostProc: swap_registers t8:t9 */
/* @PostProc: swap_registers t7:t8 */
/* @PostProc: swap_registers t6:t7 */
/* @PostProc: patch_insn_reg_field 0x10:rt:t7:t6 0x14:rs:t7:t6 0x14:rt:t7:t6 0x1c:rs:t7:t6 0x2c:rt:t9:t8 0x30:rs:t9:t8 0x30:rt:t9:t8 0x34:rt:t8:t7 0x38:rs:t8:t7 0x38:rt:t8:t7 0x40:rd:t1:t0 0x40:rs:t9:t8 0x48:rd:t2:t1 0x48:rs:t8:t7 0x48:rt:t1:t0 0x50:rt:t2:t1 */
#include "types.h"

extern s32 serifu_no;
extern s8 date2_mode_no;
extern u8 date2_mode_done_flag;

void date2_girl_bustup_read2(void);
void date2_girl0_bustup_read(void);
void date2_girl_res_set(void);

void date2_flag_set(void)
{
    s32 t6;
    s8 shift;
    u8 bits;

    t6 = serifu_no;
    if (t6 != 0) {
        date2_girl_bustup_read2();
        shift = date2_mode_no;
        bits = date2_mode_done_flag;
        date2_mode_done_flag = bits | (u8)(1 << shift);
    } else {
        date2_girl0_bustup_read();
    }
    date2_girl_res_set();
}
