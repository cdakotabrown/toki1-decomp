/* @SymbolStart: 0x801428D8 */
/* @SymbolSize: 124 */
/* @PostProc: patch_insn_reg_field 0x10:rt:t7:v0 0x14:rs:t7:v0 0x14:rt:t7:v0 0x1c:rd:t8:t6 0x1c:rt:t7:v0 0x20:rd:t8:t6 0x20:rs:t8:t6 0x20:rt:t7:v0 0x24:rd:t8:t6 0x24:rt:t8:t6 0x30:rd:t8:t6 0x30:rs:t8:t6 0x30:rt:t7:v0 0x38:rd:t9:v0 0x38:rs:t8:t6 0x3c:rt:t6:t7 0x40:rt:t0:t8 0x44:rt:t0:t8 0x48:rt:t6:t7 0x4c:rt:t9:v0 */
/* Address: 0x801428E0 (true entry 0x801428D8, 2 insns hoisted above .sym) */
#include "types.h"

extern void dtd_on_tpage(s32, s32, s32, s32, s32);
extern void _sprite_set_box_shade(s32, s32, s32, s32, s32, s32, s32);

extern u8 screen_bri;

void etc_card_shade_disp(void)
{
    s32 seed;

    seed = screen_bri;
    _sprite_set_box_shade(-140, 44, 296, 64, 4, seed * 0x10101 + 0x141414, 129);
    dtd_on_tpage(0, 0, 4, 2, 0);
}
