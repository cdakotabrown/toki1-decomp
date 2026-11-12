/* Function: menu_step_main7 */
/* Address: 0x8005D688 */
/* Size: 292 bytes (73 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack_complete --old 32 --new 56 */
/* @PostProc: remap_stack_slots --map 28:44 */
/* @PostProc: swap_instructions --offset1 0x6c --offset2 0x74 */
#include "common.h"

extern void menu_check(s32 a0, s16 a1, s16 a2);
extern void menu_bar_show(s32 a0);
extern void menu_cursor_move(s32 a0);
extern void k_disp_inc(void);
extern void card_step_init(void);
extern void sub_sub_step_up(void);
extern void album_info_disp(s32 a0, s32 a1);
extern void goto_sub_sub_step(s32 a0);
extern void cursor_work_copy(s32 a0);
extern void goto_sub_step(s32 a0);
extern void slot_box_disp(void);

extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern s8 menu_csr;
extern s32 pad_trig;
extern u8 album_status[];
extern volatile u8 now_album;
extern u8 card_sys_present;
extern u8 last_album;

void menu_step_main7(void) {
    s32 sp2C;
    s32 temp_v1;

    menu_check(1, cursor_cw_x, cursor_cw_y);
    menu_bar_show(1);
    menu_cursor_move(1);
    sp2C = menu_csr + 1;
    k_disp_inc();
    temp_v1 = pad_trig;
    if (temp_v1 & 0x20) {
        if (album_status[sp2C] != 0) {
            if (sp2C == now_album) {
                card_step_init();
                sub_sub_step_up();
            } else {
                now_album = (u8)sp2C;
                if (card_sys_present != 0) {
                    last_album = now_album;
                    album_info_disp(0, 1);
                } else {
                    goto_sub_sub_step(0xB0);
                }
            }
        }
    } else if (temp_v1 & 0x40) {
        cursor_work_copy(1);
        goto_sub_step(0);
        goto_sub_sub_step(2);
    }
    k_disp_inc();
    slot_box_disp();
}
