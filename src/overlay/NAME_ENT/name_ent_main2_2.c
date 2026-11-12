/* @CompileEnv: IDO52_COPYPROP_VAR=-12 */
/* @PostProc: swap_branch_operands --offsets 0x5c,0xe8 */
/* @CompileFlags: -Wc,-pic1 */
/* name_ent_main2_2 - NAME_ENT overlay, Address: 0x8013D054, Size: 396 bytes */
#include "types.h"
#include "tokimeki/class_w.h"

extern void menu_check(s32, s16, s16);
extern void menu_cursor_move(s32);
extern void device_cmd_send(s32);
extern void goto_sub_sub_step(s32);
extern void text_sys_init(void);
extern void name_ent_album_info_disp(void);
extern void k_sub_reset_point_set(void);
extern void k_sub_disp_start(s32);
extern void name_ent_cursor_set(s32);
extern void name_hilite_set(s32);
extern void name_ent_status_disp(void);
extern void name_phase_disp(void);
extern void name_ent_menu_select(void);

extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern u32 pad_trig;
extern volatile s8 now_icon;
extern u8  album_status1[];
extern u8  last_album;
extern u8  now_album;
extern CLASS_W class_w[];

void name_ent_main2_2(void)
{
    s32 pad;
    s32 raw;
    s32 cursor;

    menu_check(0, cursor_cw_x, cursor_cw_y);

    pad_trig = pad_trig & (u32)(-257);

    menu_cursor_move(0);

    raw = now_icon;
    cursor = raw;
    if (raw != -1) {
        class_w[119].disp_flag |= 0x80;
        class_w[119].x.half[1] = (s16)((cursor / 7) * 31 - 130);
        class_w[119].y.half[1] = (s16)((cursor % 7) * 16 - 11);
    }

    if ((pad_trig & 0x0820) && raw != -1 && album_status1[cursor] == 1) {
        if (cursor + 1 == last_album) {
            device_cmd_send(1281);
            goto_sub_sub_step(3);
            now_album = last_album;
        } else {
            last_album = (u8)(raw + 1);
            text_sys_init();
            name_ent_album_info_disp();
            k_sub_reset_point_set();
            k_sub_disp_start(0);
            name_ent_cursor_set(cursor);
        }
    }

    name_hilite_set(1);
    name_ent_status_disp();
    name_phase_disp();
    name_ent_menu_select();
}
