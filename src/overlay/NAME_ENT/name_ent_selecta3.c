/* @CompileEnv: IDO52_COPYPROP_VAR=-12 */
/* @PostProc: swap_branch_operands --offsets 0x5c,0xe8 */
/* @CompileFlags: -Wc,-pic1 */
/* name_ent_selecta3 - NAME_ENT overlay, Address: 0x8013E5AC, Size: 364 bytes (91 insns) */
#include "types.h"
#include "tokimeki/class_w.h"

extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern u32 pad_trig;
extern s8 now_icon;
extern u8 now_album;
extern u8 album_status1[];
extern u8 card_sys_present;
extern CLASS_W class_w[];

extern void menu_check(s32, s16, s16);
extern void menu_cursor_move(s32);
extern s32 device_cmd_send(s32);
extern void sub_sub_step_up(void);
extern void goto_sub_sub_step(s32);
extern void name_ent_menu_select(void);

void name_ent_selecta3(void)
{
    s32 pad;
    s32 raw;
    s32 sel;

    menu_check(0, cursor_cw_x, cursor_cw_y);
    pad_trig &= ~0x100;
    menu_cursor_move(0);

    raw = now_icon;
    sel = raw;
    if (raw != -1) {
        class_w[119].disp_flag |= 0x80;
        class_w[119].x.half[1] = (s16)((sel / 7) * 31 - 130);
        class_w[119].y.half[1] = (s16)((sel % 7) * 16 - 11);
    }

    if ((pad_trig & 0x820) != 0) {
        if (raw != -1) {
            device_cmd_send(1281);
            now_album = (u8)(sel + 1);
            if (album_status1[sel] != 0) {
                sub_sub_step_up();
                now_album = (u8)(now_icon + 1);
            } else if (card_sys_present != 0) {
                goto_sub_sub_step(168);
            } else {
                goto_sub_sub_step(169);
            }
        }
    }

    name_ent_menu_select();
}
