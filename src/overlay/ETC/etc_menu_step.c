/* @SymbolSize: 240 */
/* @CompileEnv: IDO52_AS1_NO_FALLTHRU_FILL=all */
/* Address: 0x8014971C */
/* Size: 240 bytes (60 instructions) */
#include "types.h"

extern void goto_sub_step(s32);
extern void menu_check(s32, s16, s16);
extern void menu_cursor_move(s32);
extern void menu_bar_show(s32);
extern u8 sinro;
extern s32 pad_trig;
extern volatile s8 now_icon;
extern s16 cursor_cw_x;
extern s16 cursor_cw_y;

void etc_menu_step(void)
{
    s32 v0;

    menu_check(0, cursor_cw_x, cursor_cw_y);
    menu_bar_show(0);
    menu_cursor_move(0);
    if ((pad_trig & 0x20) == 0) {
        goto end;
    }
    v0 = now_icon;
    switch (v0) {
    case 0:
        if (sinro != 0) {
            goto_sub_step(2);
            goto end;
        }
        goto_sub_step(1);
        goto end;
    case 1:
        if (sinro != 0) {
            goto_sub_step(1);
            goto end;
        }
        goto_sub_step(2);
        goto end;
    case 2:
        goto_sub_step(3);
        break;
    }
end:
    ;
}
