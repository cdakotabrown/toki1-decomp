/* @SymbolSize: 372 */
/* @RodataBase: 0x8015CC70 */
/* Address: 0x8013C448 */
/* Size: 352 bytes (88 insns) */
/* @PostProc: swap_registers t6:t7 */
/* @PostProc: patch_insn_reg_field 0x10:rt:t7:t6 0x18:rt:t7:t6 0x1c:rt:v0:t7 0x20:rs:v0:t7 0x20:rt:v0:t7 0x28:rs:v0:t7 0x34:rt:v0:t7 0x54:rt:t9:t8 0x5c:rs:t9:t8 0xa4:rt:t0:v0 0xa8:rs:t0:v0 0x100:rt:t1:v0 0x104:rs:t1:v0 */
#include "types.h"

extern volatile s32 bunka_girl;
extern volatile s32 bunka_bukatu;

extern void goto_sub_step(s32 a0);
extern void game_reset(void);

#define now_year (*(volatile u8 *)0x800EE79E)

void bunkasai_girl3_cl_scene_select(void) {
    s32 sel;
    u8 val;

    bunka_girl = 3;
    sel = bunka_bukatu;

    switch (sel) {
    case 0:
        if (now_year == 97) {
            goto_sub_step(67);
        } else {
            goto_sub_step(66);
        }
        break;
    case 1:
        goto_sub_step(68);
        break;
    case 2:
        goto_sub_step(69);
        break;
    case 3:
        switch (now_year) {
        case 95:
            goto_sub_step(70);
            break;
        case 96:
            goto_sub_step(72);
            break;
        case 97:
            goto_sub_step(73);
            break;
        }
        break;
    case 4:
        switch (now_year) {
        case 95:
            goto_sub_step(74);
            break;
        case 96:
            goto_sub_step(75);
            break;
        case 97:
            goto_sub_step(76);
            break;
        }
        break;
    case 5:
        goto_sub_step(77);
        break;
    default:
        game_reset();
        break;
    }
}
