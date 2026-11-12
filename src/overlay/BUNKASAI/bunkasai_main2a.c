/* @SymbolStart: 0x801354B0 */
/* @SymbolSize: 164 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=5:v1 IDO52_AS1_TIE_REJECT=3,5,6,7 */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x18 */
/* @PostProc: patch_insn_reg_field 0x1c:rt:t7:t8 0x34:rs:t7:t8 0x34:rt:t8:t9 0x40:rt:t8:t9 0x58:rt:t9:t0 0x5c:rs:t9:t0 0x5c:rt:t9:t0 0x64:rs:t9:t0 */
#include "types.h"
extern s32 bunka_girl;
extern s32 bunka_bukatu;

extern u8 now_year;
extern u8 girl1_flag;
extern void goto_sub_step(s32 a0);
extern void game_reset(void);

void bunkasai_main2a(void)
{
    s32 v1;
    s32 t8;

    bunka_girl = 1;
    v1 = bunka_bukatu;
    t8 = girl1_flag;
    girl1_flag = t8 | 4;

switch (v1) {
    case 0:
        if (now_year == 97) {
            goto_sub_step(44);
        } else {
            goto_sub_step(43);
        }
        break;
    case 1:
        goto_sub_step(45);
        break;
    default:
        game_reset();
        break;
    }
}
