/* @SymbolSize: 372 */
/* @RodataBase: 0x8015C060 */
/* @PostProc: patch_insn_reg_field 0x1c:rt:v0:t7 0x20:rs:v0:t7 0x20:rt:v0:t7 0x28:rs:v0:t7 0x34:rt:v0:t7 0x54:rt:t9:t8 0x5c:rs:t9:t8 0xa4:rt:t0:v0 0xa8:rs:t0:v0 0x100:rt:t1:v0 0x104:rs:t1:v0 */
/* Address: 0x801362B4 */
/* Size: 352 bytes (88 insns) */
#include "types.h"

extern volatile s32 bunka_girl;
extern volatile s32 bunka_bukatu;

extern void goto_sub_step(s32 a0);
extern void game_reset(void);

#define now_year (*(volatile u8 *)0x800EE79E)

void bunkasai_girl4_cl_scene_select(void) {
    s32 sel;
    s32 val2;
    s32 val3;

    bunka_girl = 4;
    sel = bunka_bukatu;

    switch (sel) {
    case 0:
        if (now_year == 97) {
            goto_sub_step(87);
        } else {
            goto_sub_step(86);
        }
        break;
    case 1:
        goto_sub_step(88);
        break;
    case 2:
        goto_sub_step(89);
        break;
    case 3:
        val2 = now_year;
        switch (val2) {
        case 95:
            goto_sub_step(90);
            break;
        case 96:
            goto_sub_step(92);
            break;
        case 97:
            goto_sub_step(89);
            break;
        }
        break;
    case 4:
        val3 = now_year;
        switch (val3) {
        case 95:
            goto_sub_step(93);
            break;
        case 96:
            goto_sub_step(94);
            break;
        case 97:
            goto_sub_step(95);
            break;
        }
        break;
    case 5:
        goto_sub_step(96);
        break;
    default:
        game_reset();
        break;
    }
}
