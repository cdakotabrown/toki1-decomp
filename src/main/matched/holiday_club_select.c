/* Function: holiday_club_select */
/* Address: 0x800786F8 */
/* Size: 336 bytes (84 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @CompileFlags: -Wc,-pic1 */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=12:t8,13:t9,14:t0,15:t1,16:t2 */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

typedef struct {
    u8 pad[0x48];
} TempStruct;

extern s16 cursor_cw_y;
extern s16 cursor_cw_x;
extern u32 pad_trig;
extern s8 menu_csr;
extern u8 now_sub_sub_step;
extern u8 screen_w[];
extern u16 basyo_pack;
extern u32 *album_info_disp_tbl1[];
extern u8 holiday_club_select_tbl0[];

extern void menu_check(s32, s16, s16);
extern void menu_bar_show(s32);
extern void menu_cursor_move(s32);
extern void goto_sub_step(s32);
extern void goto_sub_sub_step(u8);
extern void text_sys_init(void);
extern void strcpy(TempStruct *, u32 *);
extern void strcat(TempStruct *, u8 *);
extern void sndi(TempStruct *, s32, s32);
extern void sub_sub_step_up(void);
extern void k_disp_start(s32);

void holiday_club_select(void) {
    TempStruct temp;
    volatile u8 *p;

    menu_check(1, cursor_cw_x, cursor_cw_y);
    menu_bar_show(1);
    menu_cursor_move(1);

    if (pad_trig & 0x40) {
        goto_sub_step(0);
    }

    if (!(pad_trig & 0x20)) {
        return;
    }

    switch (menu_csr) {
    case 0:
        sub_sub_step_up();
        return;
    case 1:
        break;
    default:
        return;
    }

    goto_sub_sub_step((u8)(now_sub_sub_step + 2));
    text_sys_init();
    strcpy(&temp, album_info_disp_tbl1[(u32)basyo_pack >> 12]);
    strcat(&temp, holiday_club_select_tbl0);
    sndi(&temp, 0, 255);

    p = screen_w;
    *(p + 1647 + ((u32)basyo_pack >> 12) * 4) |= 1;
    p[245] = (p[245] & 0xFF0F) | 0xB0;

    k_disp_start(1);
}
