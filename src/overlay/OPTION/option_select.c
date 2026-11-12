/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_COPYPROP_VAR=-12 IDO52_FORCE_WEB_COLOR=70:v1 */
/* option_select - OPTION overlay, Address: 0x801383D8, Size: 556 bytes */
#include "types.h"
#include "tokimeki/class_w.h"

extern u8  album_work[];
extern u8  screen_w[];
extern u8  cw96_active[][68];
extern volatile s16 cw159_scale_x;
extern volatile s16 cw159_scale_y;
extern s16 cursor_cw_x;
extern volatile s16 cursor_cw_y;
extern s8  menu_csr;
extern u32 pad_trig;
extern u8  schedule_kind;
extern u8  schedule_cursor;
extern u8  now_girl;
extern u8  now_sub_sub_step;
extern CLASS_W class_w[];

extern void menu_check();
extern void menu_cursor_move();
extern void option_menu_box_show();
extern void class_clear_all();
extern void text_sys_init();
extern void device_load_start();
extern void device_cmd_send();
extern void goto_sub_sub_step();

void option_select(void)
{
    s32 i;
    s32 idx;
    s32 sel;
    s32 n;
    u32 uc;

    for (i = 0; i < 14; i++) {
        if (album_work[i + 20] != 0 || screen_w[i + 1340] != 0) {
            cw96_active[0][i * 68 + 1023] = 128;
        }
    }
    class_w[159].disp_flag = 128;
    cw159_scale_x = 4096;
    cw159_scale_y = 4096;
    menu_check(1, cursor_cw_x, cursor_cw_y);
    menu_cursor_move(1);
    option_menu_box_show();

    idx = menu_csr;
    for (i = 0, n = 0; i < 14; i++, n += 2) {
        if (i == idx) {
            *(s16 *)&cw96_active[i][1044] = n + 91;
        } else {
            *(s16 *)&cw96_active[i][1044] = n + 92;
        }
    }

    if (pad_trig & 0x20) {
        sel = menu_csr;
        if (album_work[menu_csr + 20] != 0 ||
            screen_w[menu_csr + 1340] != 0) {
            if (sel >= 0 && sel < 14) {
                uc = sel & 0xFF;
                schedule_kind = sel;
                schedule_cursor = 0;
                now_girl = uc;
                if (uc < 13) {
                    class_clear_all(0);
                    text_sys_init();
                    device_load_start(1, 0xBFA6, 0xBF87, 0xCE7B, 0xCE41, 0xCE2C);
                    device_cmd_send(1281);
                    goto_sub_sub_step(33);
                } else {
                    class_clear_all(0);
                    text_sys_init();
                    device_cmd_send(1281);
                    goto_sub_sub_step(35);
                }
            }
        }
    }
    if (pad_trig & 0x40) {
        menu_csr = -1;
        goto_sub_sub_step((u8)(now_sub_sub_step + 1));
    }
}
