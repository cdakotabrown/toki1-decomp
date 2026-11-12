/* Size: 84 bytes */
/* @SymbolSize: 84 */
/* @PostProc: swap_instructions --offset1 0x2c --offset2 0x34 */
/* @PostProc: swap_instructions --offset1 0x34 --offset2 0x38 */
/* @PostProc: swap_instructions --offset1 0x30 --offset2 0x38 */
/* @PostProc: patch_insn_reg_field 0x18:rt:t0:t9 0x20:rd:t7:v0 0x24:rt:t7:v0 0x24:rd:t9:t8 0x28:rs:t9:t8 0x28:rt:t7:v0 0x28:rd:t9:t8 0x2c:rt:t9:t8 0x2c:rd:t9:t8 0x30:rt:t7:v0 0x30:rd:t8:t0 0x34:rs:t0:t9 0x34:rt:t0:t9 0x3c:rt:t8:t0 0x40:rs:t9:t8 0x40:rt:t0:t9 */
#include "types.h"
extern u32 reserved_date;
extern u8 screen_w[];
extern s16 tel_girl_adjust_tbl[];
void tel_girl_adjust(void)
{
    u32 i;
    s16 d;
    u8 *p;

    i = (reserved_date << 3) >> 28;
    p = screen_w + i * 56;
    d = tel_girl_adjust_tbl[i];
    *(s16 *)(p + 450) = *(s16 *)(p + 450) - d;
    *(s16 *)(p + 454) = *(s16 *)(p + 454) + d;
}
