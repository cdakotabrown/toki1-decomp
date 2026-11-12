/* Function: save_file_select_main */
/* Address: 0x8005CD0C */
/* Size: 408 bytes (102 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @CompileFlags: -Wc,-pic1 */
/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=all IDO52_FORCE_WEB_COLOR=12:a0 IDO52_AS1_NO_COPY_PROP=1 IDO52_COPYPROP_VAR=-8 IDO52_AS1_TIE_TAKE=79 */
/* @PostProc: strip_leading_instructions --count auto */
#include "common.h"

extern void menu_check(s32 a0, s16 a1, s16 a2);
extern void menu_cursor_move(s32 a0);
extern void menu_bar_show(s32 a0);
extern void k_disp_inc(void);
extern void card_step_init(void);
extern void sub_sub_step_up(void);
extern void album_info_disp(s32 a0, s32 a1);
extern void goto_sub_sub_step(s32 a0);
extern void cursor_work_copy(s32 a0);
extern void goto_sub_step(s32 a0);
extern void save_file_box_disp(void);
extern s32 card_file_make(s32 a0);

extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern s8 menu_csr;
extern s32 pad_trig;
extern u8 album_work[];
extern volatile u8 now_album;
extern u8 card_mode;
extern u8 card_sys_present;
extern u8 last_album;

void save_file_select_main(void) {
    s32 temp_v1;
    u32 temp_v0;
    u8 *temp_a1;
    s32 sp30;
    s32 pad;

    (void)&pad;
    menu_check(1, cursor_cw_x, cursor_cw_y);
    menu_cursor_move(1);
    menu_bar_show(1);
    sp30 = menu_csr + 1;
    k_disp_inc();
    temp_v1 = pad_trig;
    if (temp_v1 & 0x40) {
        cursor_work_copy(1);
        goto_sub_step(0);
        goto_sub_sub_step(2);
        card_mode = 0;
    } else if ((temp_v1 & 0x20) && (menu_csr != -1)) {
        temp_a1 = &album_work[sp30];
        if (temp_a1[4] == 0) {
            temp_v0 = sp30 & 0xFF;
            now_album = temp_v0;
            if (card_file_make(temp_v0) == 1) {
                temp_a1[4] = 255;
                card_step_init();
                sub_sub_step_up();
            } else {
                goto_sub_sub_step(240);
            }
        } else {
            temp_v0 = now_album;
            if (sp30 == temp_v0) {
                card_step_init();
                sub_sub_step_up();
            } else {
                temp_v0 = sp30 & 0xFF;
                if (card_sys_present != 0) {
                    last_album = temp_v0;
                    now_album = temp_v0;
                    album_info_disp(0, 0);
                    menu_csr = (s8)(last_album - 1);
                } else {
                    now_album = temp_v0;
                    goto_sub_sub_step(192);
                }
            }
        }
    }
    k_disp_inc();
    save_file_box_disp();
}
