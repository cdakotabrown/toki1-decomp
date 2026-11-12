/* kangei_girl_flag_set - KANGEI overlay, Address: 0x80135464, Size: 88 bytes */
/* @SymbolStart: 0x80135448 */
/* @SymbolSize: 116 */
/* @PostProc: swap_instructions --offset1 0x24 --offset2 0x28 */
/* @PostProc: swap_instructions --offset1 0x28 --offset2 0x30 */
/* @PostProc: swap_instructions --offset1 0x2c --offset2 0x30 */
/* @PostProc: patch_insn_reg_field 0x18:rt:a1:a0 0x1c:rt:a1:a0 0x20:rt:v0:v1 0x24:rs:v0:v1 0x24:rt:v0:v1 0x28:rt:t0:t1 0x38:rd:v1:v0 0x38:rs:t7:v1 0x38:rt:v0:t7 0x3c:rs:v1:v0 0x3c:rt:t8:t9 0x44:rs:t8:t9 0x44:rt:t9:t0 0x48:rs:v1:v0 0x48:rt:t9:t0 0x4c:rs:t0:t1 0x4c:rt:t0:t1 0x54:rs:t0:t1 0x54:rt:a1:a0 0x58:rd:t1:t2 0x5c:rd:a0:v0 0x5c:rs:t1:v1 0x5c:rt:v0:t2 0x60:rs:a0:v0 0x60:rt:t2:t4 0x68:rs:t2:t4 0x68:rt:t3:t5 0x70:rs:a0:v0 0x70:rt:t3:t5 */
#include "types.h"

extern void sub_step_up(void);

extern u8 now_girl;
extern u8 D_800EF6BF_b;
extern u8 screen_w[];

void kangei_girl_flag_set(void)
{
    u8 *base;

    base = (u8 *)((u32)screen_w | 0);
    base[now_girl * 56 + 456] |= 2;
    base[D_800EF6BF_b * 56 + 456] |= 4;
    sub_step_up();
}
