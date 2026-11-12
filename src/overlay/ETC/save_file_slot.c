/* @SymbolSize: 388 */
/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_COPYPROP_VAR=-32 IDO52_FORCE_WEB_COLOR=12:a0 IDO52_COPYPROP_SITE=lod:3:deny,lod:5:deny */
/* Address: 0x80144D7C */
/* Size: 388 bytes (97 instructions) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void goto_sub_step(s32);
extern void goto_sub_sub_step(s32);
extern void k_disp_inc(void);
extern void menu_check(s32, s32, s32);
extern void menu_cursor_move(s32);
extern void menu_bar_show(s32);
extern void card_step_init(void);
extern s32 card_file_make(s32);
extern void save_file_msg_disp(void);
extern void save_file_box_disp(void);
extern void album_info_disp(s32, s32);

extern s8 menu_csr;
extern s32 pad_trig;
extern u8 now_album;
extern u8 album_status[];
extern u8 last_album;
extern u8 card_sys_present;
extern s16 cursor_cw_x;
extern s16 cursor_cw_y;

void save_file_slot(void)
{
    s32 pad[3];
    s32 slot;
    s32 res;
    s32 flags;
    s32 idx;
    s32 idx2;
    s32 cur;

    menu_check(1, cursor_cw_x, cursor_cw_y);
    menu_cursor_move(1);
    menu_bar_show(1);
    slot = menu_csr + 1;
    k_disp_inc();
    flags = pad_trig;
    if ((flags & 0x20) != 0) {
        if (menu_csr != -1) {
            if (album_status[slot] == 0) {
                idx = slot & 0xFF;
                now_album = idx;
                res = card_file_make(idx);
                if (res == 1) {
                    card_step_init();
                    sub_sub_step_up();
                } else {
                    goto_sub_sub_step(240);
                }
            } else {
                cur = now_album;
                if (slot == cur) {
                    card_step_init();
                    sub_sub_step_up();
                } else {
                    idx2 = slot & 0xFF;
                    if (card_sys_present != 0) {
                        last_album = idx2;
                        now_album = idx2;
                        album_info_disp(0, 0);
                        save_file_msg_disp();
                        menu_csr = last_album - 1;
                    } else {
                        now_album = idx2;
                        goto_sub_sub_step(192);
                    }
                }
            }
        }
    } else if ((flags & 0x40) != 0) {
        goto_sub_step(2);
    }
    k_disp_inc();
    save_file_box_disp();
}
