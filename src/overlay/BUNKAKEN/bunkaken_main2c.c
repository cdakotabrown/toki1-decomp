/* @SymbolStart: 0x8013BFE0 */
/* Address: 0x8013BFE8 */
/* Size: 196 bytes (49 insns) + 2-insn cross-prologue tail */
/* @SymbolSize: 204 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x28:rs:v0:v1 0x34:rs:v0:v1 0x80:rt:t0:t1 0x84:rd:t9:t0 0x88:rs:t9:t0 0x90:rt:t0:t1 */
#include "types.h"

extern void goto_sub_step(s32);
extern void game_reset(void);
extern s32 get_g_zyotai_low(s32);

extern u8  now_year;
extern s32 girl1_flag;
extern s32 bunka_girl;
extern s32 bunka_bukatu;

void bunkaken_main2c(void) {
    s32 sel;
    s32 v0;

    sel = bunka_bukatu;

    bunka_girl = 1;

    switch (sel) {
    case 0:
        if (now_year == 97) {
            goto_sub_step(46);
            break;
        }
        v0 = get_g_zyotai_low(1);
        if (v0 != 4) goto L_msg45;
        if ((s32)(girl1_flag << 29) < 0) goto L_msg45;
        bunka_girl = 255;
        goto_sub_step(5);
        break;
L_msg45:
        goto_sub_step(45);
        break;
    case 1:
        goto_sub_step(47);
        break;
    default:
        game_reset();
        break;
    }
}
