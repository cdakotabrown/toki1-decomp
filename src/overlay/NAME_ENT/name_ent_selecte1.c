/* @CompileEnv: IDO52_COPYPROP_VAR=-8 */
/* @PostProc: swap_branch_operands --offsets 0x5c,0xe8 */
/* @SymbolSize: 304 */
/* name_ent_selecte1 - NAME_ENT overlay, Address: 0x8013FCFC, Size: 304 bytes */
#include "types.h"
#include "tokimeki/class_w.h"

extern void menu_check(s32, s16, s16);
extern void menu_cursor_move(s32);
extern void sub_sub_step_up(void);
extern s32  device_cmd_send(s32);
extern void name_ent_menu_select(void);

extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern u32 pad_trig;
extern s8  now_icon;
extern u8  album_status1[];
extern u8  now_album;
extern CLASS_W class_w[];

void name_ent_selecte1(void)
{
    s32 raw;
    s32 cursor;

    menu_check(0, cursor_cw_x, cursor_cw_y);
    pad_trig &= ~0x100;
    menu_cursor_move(0);

    raw = now_icon;
    cursor = raw;
    if (raw != -1) {
        class_w[119].disp_flag |= 0x80;
        class_w[119].x.half[1] = (s16)((cursor / 7) * 31 - 130);
        class_w[119].y.half[1] = (s16)((cursor % 7) * 16 - 11);
    }

    if ((pad_trig & 0x820) != 0 && raw != -1 && album_status1[cursor] != 0) {
        sub_sub_step_up();
        device_cmd_send(1281);
        now_album = (u8)(now_icon + 1);
    }

    name_ent_menu_select();
}
