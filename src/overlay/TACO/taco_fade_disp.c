/* @SymbolSize: 176 */
/* @PostProc: patch_stack_complete --old 48 --new 64 */
/* @PostProc: remap_stack_slots --from 44 --to 56 */
/* @PostProc: patch_insn_reg_field 0x20:rd:t6:v0 0x24:rd:t7:v0 0x24:rs:t6:v0 0x28:rd:t8:v0 0x28:rt:t7:v0 0x30:rs:t8:v0 0x3c:rt:t9:t6 0x40:rt:t0:t7 0x44:rt:t0:t7 0x48:rt:t9:t6 0x6c:rt:t1:t8 0x70:rt:t2:t9 0x74:rt:t2:t9 0x78:rt:t1:t8 0x94:rt:t3:t0 0x98:rt:t3:t0 */
/* Address: 0x8015177C */
/* Size: 176 bytes (44 insns) */
#include "types.h"

extern void dtd_on_tpage(s32 a0, s32 a1, s32 a2, s32 a3, s32 s16);
extern void _sprite_set_box_shade(s32 a0, s32 a1, s32 a2, s32 a3,
                          s32 s16, s32 s20, s32 s24);

void taco_fade_disp(s32 bright)
{
    s32 col;

    if (bright < 256) {
        col = ((bright << 8) + bright) * 256 + bright;
    } else {
        col = 0xFFFFFF;
    }

    _sprite_set_box_shade(-256, -120, 256, 240, 1, col, 129);
    _sprite_set_box_shade(0, -120, 256, 240, 1, col, 129);
    dtd_on_tpage(0, 0, 1, 1, 1);
}
