/* @CompileEnv: IDO52_COPYPROP_VAR=-8 */
/* @PostProc: swap_branch_operands --offsets 0x64,0xf0 */
/* name_ent_selectc3 - NAME_ENT overlay, Address: 0x8013F5C4, Size: 348 bytes (87 insns) */
/* fabricated by `@PostProc: pad_trailing_nop --count 85`; that fake pad is gone. */
#include "types.h"
#include "tokimeki/class_w.h"

extern void name_ent_menu_select(void);
extern void menu_check(s32, s32, s32);
extern void menu_cursor_move(s32);
extern void sub_sub_step_up(void);
extern s32 card_file_make(s32);
extern void device_cmd_send(s32);
extern void goto_sub_sub_step(s32);
extern void goto_sub_step(s32);

extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern u32 pad_trig;
extern s8 now_icon;
extern u8 now_album;
extern u8 album_status1[];
extern CLASS_W class_w[];

void name_ent_selectc3(void)
{
    s32 raw;
    s32 sel;

    name_ent_menu_select();
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

    if (pad_trig & 0x820) {
        if (raw != -1) {
            now_album = (u8)(raw + 1);
            if (album_status1[sel] != 0) {
                sub_sub_step_up();
            } else if (card_file_make(now_album) == 1) {
                device_cmd_send(1281);
                goto_sub_sub_step(198);
            } else {
                goto_sub_step(4);
            }
        }
    }
}
