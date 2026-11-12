/* Function: system_config_menu_init (obsolete painted source; not the girl-choice menu helper) */
/* Address: 0x8005DE8C */
/* Size: 360 bytes (90 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: custom_script custom_func_8005DE8C.py */
#include "types.h"

extern void text_sys_init(void);
extern void basyo_init(void*);
extern void icon_disp_switch(s32);
extern void data_save_load_class_init(void);
extern void system_config_disp(s32);
extern s32 get_mascot_count(void);
extern void k_sub_reset_point_set(void);
extern void set_kanji_string(s32, s32, s32, void*, s32);
extern void menu_set(s32, s32, s16*, s16*, s16*, s16*);
extern void k_sub_disp_start(s32);
extern void sub_sub_step_up(void);

extern s8 now_icon;
extern s8 system_config_menu_init_str0;
extern s8 system_config_menu_init_str1;

void system_config_menu_init(void)
{
    s16 buf[32];
    s16 *p1;
    s16 *p2;
    s16 *p3;
    s16 *p4;
    s32 t1, t2, t3, t4;

    text_sys_init();
    basyo_init(&system_config_menu_init_str0);
    icon_disp_switch(0);
    data_save_load_class_init();

    p1 = &buf[24];
    p2 = &buf[16];
    t2 = -64;
    p3 = &buf[8];
    p4 = &buf[0];
    t3 = -52;
    t4 = -40;
    t1 = -28;

    do {
        p2[3] = t1;
        t1 += 48;
        p2[1] = t3;
        p2[2] = t4;
        p2[0] = t2;
        t2 += 48;
        t4 += 48;
        t3 += 48;
        p1[1] = -124;
        p3[1] = 96;
        p4[1] = 12;
        p1[2] = -124;
        p3[2] = 96;
        p4[2] = 12;
        p1[3] = -124;
        p3[3] = 96;
        p4[3] = 12;
        p1 += 4;
        p2 += 4;
        p3 += 4;
        p4 += 4;
        p1[-4] = -124;
        p3[-4] = 96;
    } while (t1 != 68);
    p4[-4] = 12;

    system_config_disp(-1);
    if (get_mascot_count() == 0) {
        buf[14] = 0;
    }
    k_sub_reset_point_set();
    set_kanji_string(-128, 50, 0, &system_config_menu_init_str1, 0);

    menu_set(0, 8, &buf[24], &buf[16], &buf[8], &buf[0]);

    t1 = now_icon;
    t2 = t1 & 7;
    if (t1 < 0 && t2 != 0) {
        t2 -= 8;
    }
    now_icon = t2;

    k_sub_disp_start(0);
    sub_sub_step_up();
}
