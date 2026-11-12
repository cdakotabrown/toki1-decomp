/* @SymbolStart: 0x801511E0 */
/* @SymbolSize: 388 */
/* @RodataBase: 0x8015F1F0 */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x18 */
/* @PostProc: swap_instructions --offset1 0x10 --offset2 0x14 */
/* @PostProc: patch_insn_reg_field 0x18:rt:t6:t7 0x20:rt:t8:t9 0x24:rs:t8:t9 0x24:rt:t8:t9 0x2c:rs:t6:t7 0x2c:rt:t7:t8 0x30:rt:t7:t8 0x38:rs:t8:t9 0x44:rd:t8:t9 0x44:rt:t8:t9 0x4c:rt:t8:t9 0x50:rt:t8:t9 0x58:rs:t8:t9 0x60:rt:t9:t0 0x64:rs:t9:t0 0x64:rt:t9:t0 0x6c:rs:t9:t0 */
/* Address: 0x80151204 (window 0x801511E0, 388 bytes / 97 insns incl. the */
#include "types.h"
extern s32 bunka_bukatu;
extern s32 bunka_girl;
extern u8 girl0_flag;

extern void goto_sub_step(s32 a0);
extern void game_reset(void);

extern u8 now_year;

void bunkasai_girl0_home_cl_scene_select(void)
{
    u8 val;
    s32 t9;
    u8 x;

    x = girl0_flag;
    bunka_girl = 0;
    t9 = bunka_bukatu;
    girl0_flag = x | 4;

switch (t9) {
    case 0:
        if (now_year == 97) {
            goto_sub_step(3);
        } else {
            goto_sub_step(2);
        }
        break;
    case 1:
        goto_sub_step(4);
        break;
    case 2:
        goto_sub_step(5);
        break;
    case 3:
        switch (now_year) {
        case 95:
            goto_sub_step(6);
            break;
        case 96:
            goto_sub_step(8);
            break;
        case 97:
            goto_sub_step(9);
            break;
        }
        break;
    case 4:
        switch (now_year) {
        case 95:
            goto_sub_step(10);
            break;
        case 96:
            goto_sub_step(11);
            break;
        case 97:
            goto_sub_step(12);
            break;
        }
        break;
    case 5:
        goto_sub_step(13);
        break;
    default:
        game_reset();
        break;
    }
}
