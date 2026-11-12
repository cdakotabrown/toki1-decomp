/* Function: join_club_select */
/* Address: 0x8006084C */
/* Size: 252 bytes (63 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @CompileEnv: IDO52_AS1_XBB_SKIP_CAND=1,99 IDO52_AS1_NO_FALLTHRU_FILL=0,99 IDO52_UGEN_TEMP_REG=7:t4,8:t5,9:t6,10:t7 */
/* @PostProc: strip_leading_instructions --count auto */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern s32 pad_trig;
extern u8 now_step;
extern s8 menu_csr;
extern u8 byouki_kind;

extern void menu_check(s32, s16, s16);
extern void menu_cursor_move(s32);
extern void menu_bar_show(s32);
extern void goto_sub_step(s32);
extern void goto_sub_sub_step(s32);
extern void sub_sub_step_up(void);

void join_club_select(void) {
    menu_check(1, cursor_cw_x, cursor_cw_y);
    menu_cursor_move(1);
    menu_bar_show(1);

    if (pad_trig & 0x40) {
        goto_sub_step(0);
        if (now_step == 52) {
            goto_sub_sub_step(2);
        }
    }

    if (pad_trig & 0x20) {
        if (menu_csr == 11) {
            goto_sub_step(0);
            if (now_step == 52) {
                goto_sub_sub_step(2);
            }
        } else if (menu_csr != -1) {
            byouki_kind = (menu_csr << 4) | (byouki_kind & 0xFF0F);
            sub_sub_step_up();
        }
    }
}
