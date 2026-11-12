/* Debug Symbol: menu_step_main2 (from o.bin, high match) */
/* Address: 0x8005C68C */
/* Size: 184 bytes (46 instructions) */
/* Region: GCC (< 0x80081000, OR encoding) */
/* Compiler: IDO 5.2 via ido-static-recomp */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern s32 pad_trig;
extern s8 menu_csr;

extern void menu_check(s32 arg0, s32 arg1, s32 arg2);
extern void menu_cursor_move(s32 arg0);
extern void menu_bar_show(s32 arg0);
extern void cursor_work_copy(s32 arg0);
extern void goto_sub_step(s32 arg0);
extern void goto_sub_sub_step(s32 arg0);

void menu_step_main2(void)
{
    s32 val;
    s32 v2;

    menu_check(1, cursor_cw_x, cursor_cw_y);
    menu_cursor_move(1);
    menu_bar_show(1);

    val = pad_trig;

    if ((val & 0x40) != 0) {
        cursor_work_copy(1);
        goto_sub_step(0);
        goto_sub_sub_step(2);
        return;
    }

    if ((val & 0x20) == 0) {
        return;
    }

    v2 = menu_csr;
    switch (v2) {
    case 0:
        goto_sub_sub_step(3);
        break;
    case 1:
        goto_sub_sub_step(6);
        break;
    }
}
