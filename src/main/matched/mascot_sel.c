/* Address: 0x800605F8 */
/* Size: 324 bytes (81 instructions) */
/* Region: Game (0x80041000-0x80086000) */
/* Compiler: IDO 5.2 (-cckr) */
/* Compiler: IDO 5.2 */
/* @CompileEnv: IDO52_FORCE_WEB_SPILL=6 IDO52_COPYPROP_VAR=-4 IDO52_COPYPROP_SITE=lod:1:deny,lod:5:deny IDO52_CMOT_KILL_AVAIL=13 */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern s32 pad_trig;
extern s8 menu_csr;
extern u8  mascot_flag2[];
extern u8  mascot_flag[];

void mascot_sel()
{
    int i;
    int flags;
    int j;

    menu_check(1, cursor_cw_x, cursor_cw_y);
    menu_bar_show(1);
    menu_cursor_move(1);
    k_disp_inc();

    j = menu_csr;
    if (j != -1) {
        mascot_init(0);
        if (mascot_flag[i = menu_csr] != 0 || mascot_flag2[i = menu_csr] != 0) {
            mascot_init(i + 1);
        }
    }

    flags = pad_trig;
    i = menu_csr;
    if (flags & 0x40) {
        goto_sub_sub_step(0);
        return;
    }
    if (flags & 0x20) {
        if (i == -1) {
            return;
        }
        if (i == 13) {
            goto_sub_sub_step(0);
            return;
        }
        if (mascot_flag[i] != 0 || mascot_flag2[menu_csr] != 0) {
            mascot_init(0);
            i = menu_csr;
            mascot_init(i + 1);
            sub_sub_step_up();
        }
    }
}
