/* option_mouse_menu_reset - OPTION overlay, Address: 0x8013539C, Size: 180 bytes (45 insns) */
/* @PostProc: patch_stack_complete --old 96 --new 112 */
/* @PostProc: remap_stack_slots --from 64 --to 68 --from 70 --to 74 --from 72 --to 76 --from 80 --to 84 --from 88 --to 92 */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x20 */
/* @PostProc: swap_instructions --offset1 0x20 --offset2 0x24 */
/* @PostProc: swap_instructions --offset1 0x24 --offset2 0x28 */
/* @PostProc: swap_instructions --offset1 0x28 --offset2 0x2c */
/* @PostProc: swap_instructions --offset1 0x44 --offset2 0x50 */
/* @PostProc: swap_instructions --offset1 0x4c --offset2 0x50 */
/* @PostProc: patch_insn_reg_field 0x18:rt:a1:v1 0x2c:rt:v1:a1 0x40:rt:v1:a1 0x44:rs:v1:a1 0x44:rt:v1:a1 0x48:rs:a1:v1 0x48:rt:a1:v1 0x54:rs:a1:v1 */
/* @SymbolSize: 180 */
/* @CompileFlags: -Wo,-loopunroll,0 */
#include "types.h"

extern u8 cw116_disp;
extern u8 cw116_clut;
extern void menu_set();
extern void option_menu_box_show();
extern void goto_sub_sub_step();

void option_mouse_menu_reset(void)
{
    s16 buf120[4];
    s16 bufval[4];
    s16 buf108[4];
    s16 buf16[4];
    s32 i;
    s32 val;
    u8 t;

    val = -32;
    for (i = 0; i < 3; i++) {
        bufval[i] = val;
        val += 40;
        buf120[i] = -120;
        buf108[i] = 108;
        buf16[i] = 16;
    }

    menu_set(1, 3, buf120, bufval, buf108, buf16);
    option_menu_box_show();

    t = cw116_disp & 0xFF7F;
    cw116_disp = t;
    cw116_clut = 0;

    goto_sub_sub_step(1);
}
