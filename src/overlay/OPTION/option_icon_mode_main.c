/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=all IDO52_FORCE_WEB_COLOR=12:a3 IDO52_AS1_TIE_TAKE=12 */
/* @CompileFlags: -Wc,-pic1 */
/* @SymbolStart: 0x80133F98 */
/* @SymbolSize: 784 */
/* 1. THE WALL WAS SELF-INFLICTED.  `@CompileEnv: IDO52_AS1_NO_LA_PULL=1` */
#include "types.h"

extern void menu_check();
extern void menu_cursor_move();
extern void option_menu_box_show();
extern void icon_init();
extern void icon_menu_set();
extern void sub_sub_step_up();
extern void device_cmd_send();
void option_icon_slide_out(void);

typedef struct {
    s16 x;
    s16 y;
} OptPt;

extern volatile u8  cw106_disp;
extern volatile s8  menu_csr;
extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern u8  cw96_active[];
extern volatile u8  icon_mode;
extern u8  icon_num;
extern u32 pad_trig;
extern u8  step_mode;
extern OptPt icon_xy[];

void option_icon_mode_main(void)
{
    s32 cur;
    s32 k;
    s32 j;
    s32 saved;
    s32 old;
    u8 *p;
    u8 *q;

    cw106_disp = 128;
    old = menu_csr;
    menu_check(1, cursor_cw_x, cursor_cw_y, old);
    menu_cursor_move(1);
    option_menu_box_show();

    cur = menu_csr;
    q = cw96_active + cur * 68;
    for (p = cw96_active, k = 0; p < cw96_active + 204; p += 68) {
        if (p == q) {
            *(s16 *)(p + 772) = (s16)(k + 21);
        } else {
            *(s16 *)(p + 772) = (s16)(k + 22);
        }
        k += 2;
    }

    if (old != cur) {
        switch (cur) {
        case 0:
            saved = icon_mode;
            icon_mode = 0;
            icon_init();
            for (j = 0; j < 16; j++) { icon_xy[j].x += 200; }
            icon_menu_set(icon_num);
            icon_mode = (u8)saved;
            cur = menu_csr;
            break;
        case 1:
            saved = icon_mode;
            icon_mode = 1;
            icon_init();
            for (j = 0; j < 16; j++) { icon_xy[j].x += 200; }
            icon_menu_set(icon_num);
            icon_mode = (u8)saved;
            cur = menu_csr;
            break;
        }
    }

    if ((pad_trig & 0x20) != 0) {
        switch (cur) {
        case 0:
            icon_mode = 3;
            option_icon_slide_out();
            sub_sub_step_up();
            device_cmd_send(1281);
            break;
        case 1:
            icon_mode = 4;
            option_icon_slide_out();
            sub_sub_step_up();
            device_cmd_send(1281);
            break;
        case 2:
            icon_mode = 5;
            option_icon_slide_out();
            sub_sub_step_up();
            device_cmd_send(1281);
            break;
        }
    }

    if ((pad_trig & 0x40) != 0) {
        option_icon_slide_out();
        sub_sub_step_up();
        step_mode = 2;
    }
}

void option_icon_slide_out(void)
{
    s32 j;

    for (j = 0; j < 16; j++) { icon_xy[j].x -= 200; }
}
