/* @PostProc: patch_insn_reg_field 0x1c:rt:v0:t7 0x20:rs:v0:t7 0x20:rt:v0:t7 0x28:rs:v0:t7 0x34:rt:v0:t7 0x54:rt:t9:t8 0x5c:rs:t9:t8 0xa4:rt:t0:v0 0xa8:rs:t0:v0 0x100:rt:t1:v0 0x104:rs:t1:v0 */
/* @SymbolSize: 372 */
/* @RodataBase: 0x8015EC30 */
/* Address: 0x8014DDA4 */
/* Size: 352 bytes (88 insns) */
#include "types.h"

extern volatile s32 bunka_girl;
extern volatile s32 bunka_bukatu;

extern void goto_sub_step(s32 a0);
extern void game_reset(void);

#define now_year (*(volatile u8 *)0x800EE79E)

void bunkasai_main6d(void) {
    s32 sel;
    u8 val;

    bunka_girl = 6;
    sel = bunka_bukatu;

    switch (sel) {
    case 0:
        if (now_year == 97) {
            goto_sub_step(111);
        } else {
            goto_sub_step(110);
        }
        break;
    case 1:
        goto_sub_step(112);
        break;
    case 2:
        goto_sub_step(113);
        break;
    case 3:
        switch (now_year) {
        case 95:
            goto_sub_step(114);
            break;
        case 96:
            goto_sub_step(116);
            break;
        case 97:
            goto_sub_step(113);
            break;
        }
        break;
    case 4:
        switch (now_year) {
        case 95:
            goto_sub_step(117);
            break;
        case 96:
            goto_sub_step(118);
            break;
        case 97:
            goto_sub_step(119);
            break;
        }
        break;
    case 5:
        goto_sub_step(120);
        break;
    default:
        game_reset();
        break;    }
}
