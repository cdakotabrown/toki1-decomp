/* @CompileEnv: IDO52_COPYPROP_VAR=-8 */
/* @PostProc: swap_branch_operands --offsets 0x5c,0xe8 */
/* name_ent_main2_21 - NAME_ENT overlay, Address: 0x8013D658, Size: 352 bytes */
#include "types.h"
#include "tokimeki/class_w.h"

extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern u32 pad_trig;
extern s8 now_icon;
extern u8 now_album;
extern u8 album_status1[];
extern CLASS_W class_w[];

extern void menu_check(s32, s16, s16);
extern void menu_cursor_move(s32);
extern void goto_sub_sub_step(s32);
extern void name_hilite_set(s32);
extern void name_ent_status_disp(void);
extern void name_phase_disp(void);
extern void name_ent_menu_select(void);

void name_ent_main2_21(void)
{
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

    if ((pad_trig & 0x820) != 0 && raw != -1 && album_status1[sel] == 1) {
        if (sel + 1 == now_album) {
            goto_sub_sub_step(36);
        } else {
            goto_sub_sub_step(34);
            now_album = (u8)(now_icon + 1);
        }
    }

    name_hilite_set(1);
    name_ent_status_disp();
    name_phase_disp();
    name_ent_menu_select();
}
