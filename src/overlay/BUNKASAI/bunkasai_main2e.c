/* @RodataBase: 0x8015B910 */
/* @SymbolSize: 372 */
/* @PostProc: patch_insn_reg_field 0x1c:rt:v0:t7 0x20:rs:v0:t7 0x20:rt:v0:t7 0x28:rs:v0:t7 0x34:rt:v0:t7 0x54:rt:t9:t8 0x5c:rs:t9:t8 0xa4:rt:t0:v0 0xa8:rs:t0:v0 0x100:rt:t1:v0 0x104:rs:t1:v0 */
/* Address: 0x80132480 */
/* Size: 372 bytes (93 insns) -- MERGED with former func_80132494. */
#include "types.h"

extern void goto_sub_step(s32 a0);
extern void game_reset(void);
extern volatile s32 bunka_girl;
extern volatile s32 bunka_bukatu;

#define now_year (*(volatile u8 *)0x800EE79E)

void bunkasai_main2e(void) {
    s32 val2;
    s32 val3;
    s32 sel;

    bunka_girl = 2;
    sel = bunka_bukatu;
    switch (sel) {
    case 0:
        if (now_year == 97) {
            goto_sub_step(48);
        } else {
            goto_sub_step(47);
        }
        break;
    case 1:
        goto_sub_step(49);
        break;
    case 2:
        goto_sub_step(50);
        break;
    case 3:
        val2 = now_year;
        switch (val2) {
        case 95:
            goto_sub_step(51);
            break;
        case 96:
            goto_sub_step(53);
            break;
        case 97:
            goto_sub_step(54);
            break;
        }
        break;
    case 4:
        val3 = now_year;
        switch (val3) {
        case 95:
            goto_sub_step(55);
            break;
        case 96:
            goto_sub_step(56);
            break;
        case 97:
            goto_sub_step(57);
            break;
        }
        break;
    case 5:
        goto_sub_step(58);
        break;
    default:
        game_reset();
        break;
    }
}
