/* Address: 0x8005A8F8 */
/* Size: 1252 bytes (313 instructions) */
/* Region: Game (IDO 5.2) */
/* @CompileEnv: IDO52_COPYPROP_VAR=-4,-8 IDO52_COPYPROP_SITE=lod:1:deny,lod:2:deny IDO52_FORCE_WEB_COLOR=9:a0 IDO52_AS1_TIE_SRC_ORDER=bb:14 */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u8 pad0[102];
    u8 a[5];
    u8 b[137];
    u16 w244;
} Save;

extern Save screen_w;
extern u8 back_clear_r;
extern u8 back_clear_g;
extern u8 back_clear_b;
extern u8 now_year;
extern u8 now_month;
extern u8 now_day;
extern u16 basyo_pack;
extern u8 byouki_kind;
extern u16 byouki_state;
extern u16 bukatu_aux6;
extern u8 icon_num;
extern s32 sched_icon;
extern u8 now_icon;
extern u8 p_sei[];
extern u8 step34_init2_str0[];
extern u8 step34_init2_str1[];
extern u8 step34_init2_str2[];
extern u8 step34_init2_str3[];
extern u8 step34_init2_str4[];
extern u8 step34_init2_str5[];
extern u8 step34_init2_str6[];
extern u8 step34_init2_str7[];
extern u8 step34_init2_str8[];
extern u8 step34_init2_str9[];

extern void back_clear_switch();
extern void class_clear_f();
extern void prim_init();
extern void text_sys_init();
extern void basyo_init2();
extern void gnsx();
extern void parameter_show_init();
extern void hizuke_init();
extern void message_window_init();
extern void parameter_disp_switch();
extern void tpage_buf_clear();
extern void icon_init();
extern void icon_menu_set();
extern void icon_can_use_clear();
extern void icon_can_use_set();
extern void sndisp();
extern void cursor_image_load();
extern void k_disp_start();
extern void cursor_init();
extern void menu_bar_color();
extern void step34_load();

void step34_init2()
{
    Save *sv;
    u8 *p;
    u8 m;

    back_clear_switch(0);
    class_clear_f();
    prim_init();
    back_clear_r = 0;
    back_clear_g = 0;
    back_clear_b = 0;
    text_sys_init();
    basyo_init2(step34_init2_str0);
    gnsx(p_sei);
    parameter_show_init();
    parameter_show_init();
    hizuke_init();
    message_window_init();
    parameter_disp_switch(1);
    tpage_buf_clear();

    sv = &screen_w;
    p = (u8 *)sv + now_day - 1;
    m = p[102];
    if (m == 5) {
        icon_num = 15;
        icon_init();
        icon_menu_set(icon_num);
        icon_can_use_clear(1);
        now_icon = 14;
        icon_can_use_set(14, 1);
        sndisp(step34_init2_str1, 0, 31);
    } else if (m == 8 && (sv->w244 >> 12) != 11) {
        icon_num = 14;
        icon_init();
        icon_menu_set(icon_num);
        icon_can_use_clear(1);
        icon_can_use_set(4, 1);
        now_icon = 4;
        if ((basyo_pack >> 12) == 6 && now_year == 97 && (bukatu_aux6 & 0xF) >= 8) {
            sndisp(step34_init2_str2, 0, 31);
        } else {
            sndisp(step34_init2_str3, 0, 31);
        }
    } else if (p[107] == 12 && (sv->w244 >> 12) < 6) {
        if ((byouki_kind & 0xF) != 3) {
            icon_num = 14;
            icon_init();
            icon_menu_set(icon_num);
            icon_can_use_clear(1);
            icon_can_use_set(4, 1);
            now_icon = 4;
            sndisp(step34_init2_str4, 0, 31);
        } else {
            icon_num = 14;
            icon_init();
            icon_menu_set(icon_num);
            icon_can_use_clear(1);
            icon_can_use_set(7, 1);
            now_icon = 7;
            cursor_image_load();
            sndisp(step34_init2_str5, 0, 31);
        }
    } else if (m == 11) {
        icon_num = 15;
        icon_init();
        icon_menu_set(icon_num);
        icon_can_use_clear(1);
        now_icon = 14;
        icon_can_use_set(14, 1);
        sndisp(step34_init2_str6, 0, 31);
    } else if (byouki_state & 1) {
        icon_num = 14;
        icon_init();
        icon_menu_set(icon_num);
        icon_can_use_clear(1);
        icon_can_use_set(0, 1);
        icon_can_use_set(1, 1);
        icon_can_use_set(2, 1);
        now_icon = 0;
        sndisp(step34_init2_str7, 0, 31);
    } else if ((byouki_kind & 0xF) == 3) {
        icon_num = 14;
        icon_init();
        icon_menu_set(icon_num);
        icon_can_use_clear(1);
        icon_can_use_set(7, 1);
        now_icon = 7;
        cursor_image_load();
        sndisp(step34_init2_str8, 0, 31);
    } else {
        icon_num = 14;
        icon_init();
        icon_menu_set(icon_num);
        icon_can_use_clear(0);
        now_icon = sched_icon & 0xF;
        sndisp(step34_init2_str9, 0, 31);
    }
    k_disp_start(4);
    cursor_init(0);
    if (now_year == 95 && (now_month | 0) == 4 && (now_day | 0) == 4) {
        icon_can_use_set(13, 0);
    }
    menu_bar_color(16, 0, 0, 8);
    step34_load();
}
