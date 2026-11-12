/* @RodataBase: 0x8014C884 */
#include "types.h"
#include "tokimeki/libgs_types.h"

extern u8 step_mode;
extern u8 screen_bri;
extern u8 back_clear_r;
extern u8 back_clear_g;
extern u8 back_clear_b;

extern s32 Bg;

extern u8 name_ent_load_str0[];
extern u8 name_ent_load_str1[];
extern u8 name_ent_load_str2[];
extern u8 name_ent_load_str3[];

extern void draw2d3d(s32 a, s32 b);
extern void class_switch(s32 a);
extern void set_flag_b3_high_bit(s32 a);
extern void class_clear(s32 a);
extern void class_clear_f(void);
extern void prim_init(void);
extern void text_sys_init(void);
extern void back_clear_switch(s32 a);
extern void device_cmd_send(s32 a);
extern void device_load_start(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
extern s32 device_load_poll(void);
extern void device_port_init(s32 a, s32 b);
extern void cd_read_start2(s32 a, s32 b, s32 c);
extern s32 get_CD_State2(void);
extern void bg_set(u32 *p, s32 b, s32 c, s32 d, s32 e, s32 f,
                          s32 g, s32 h, s32 i, s32 j);
extern void palette_upload_enqueue(s32 a, s32 b, s32 c, s32 d, s32 e);
extern void set_XAsector(s32 a, s32 b, s32 c);
extern void set_kanji_string(s32 a, s32 b, s32 c, u8 *d, s32 e);
extern void k_disp_start(s32 a);
extern void k_disp_switch(s32 a, s32 b);
extern void sub_sub_step_up(void);

void name_ent_load(void)
{
    s32 pad;
    u32 buf[8];
    u32 i;

    for (i = 0; i < 8; i++) buf[i] = 0x80180000 + i * 0x4000;

    switch (step_mode) {
    case 0:
        screen_bri = 128;
        draw2d3d(1, 0);
        class_switch(1);
        set_flag_b3_high_bit(0);
        class_clear(0);
        class_clear_f();
        prim_init();
        text_sys_init();
        back_clear_switch(1);
        back_clear_g = 0;
        back_clear_r = 0;
        back_clear_b = 0;
        device_cmd_send(127);
        device_load_start(1, 0xC5A0, 0xC58E, 0xCFB5, 0xCF99, 0xCF91);
        step_mode = step_mode + 1;
        break;
    case 1:
        if (device_load_poll() == 1) {
            device_port_init(1, 0);
            cd_read_start2(33, 0x80180000, 0x9FD5);
            step_mode = step_mode + 1;
        }
        break;
    case 2:
        if (get_CD_State2() & 1) {
            bg_set(buf, 0, 4, 2, 2, 511, -160, -120, 8, 9);
            palette_upload_enqueue(0x80190000, 31, 1, 3, 0);
            Bg = 0x80000000;
            cd_read_start2(33, 0x80180000, 0xA048);
            step_mode = step_mode + 1;
        }
        break;
    case 3:
        if (get_CD_State2() & 1) {
            bg_set(buf, 0, 4, 2, 2, 510, -160, -120, 8, 11);
            palette_upload_enqueue(0x80190000, 30, 1, 3, 0);
            Bg = 0x80000000;
            cd_read_start2(33, 0x80180000, 0x9FF6);
            step_mode = step_mode + 1;
        }
        break;
    case 4:
        if (get_CD_State2() & 1) {
            bg_set(buf, 0, 4, 2, 2, 509, -160, -120, 8, 13);
            palette_upload_enqueue(0x80190000, 29, 1, 3, 0);
            Bg = 0x80000000;
            cd_read_start2(65, 0x80180000, 0x9F22);
            step_mode = step_mode + 1;
        }
        break;
    case 5:
        if (get_CD_State2() & 1) {
            bg_set(buf, 1, 8, 4, 2, 494, -320, -120, 8, 21);
            palette_upload_enqueue(0x801A0000, 14, 1, 3, 0);
            (*(s32 *)&((LIBGS_BG *)&Bg)[1].attribute) = 0x51000000;
            ((LIBGS_BG *)&Bg)[1].g = 0;
            ((LIBGS_BG *)&Bg)[1].r = 0;
            ((LIBGS_BG *)&Bg)[1].b = 0;
            cd_read_start2(49, 0x801B0000, 0x9FA4);
            step_mode = step_mode + 1;
        }
        break;
    case 6:
        if (get_CD_State2() & 1) {
            cd_read_start2(65, 0x80180000, 0x9EE1);
            step_mode = step_mode + 1;
        }
        break;
    case 7:
        if (get_CD_State2() & 1) {
            bg_set(buf, 0, 8, 4, 2, 495, -160, -120, 8, 5);
            palette_upload_enqueue(0x801A0000, 15, 1, 3, 0);
            ((LIBGS_BG *)&Bg)[0].g = 0;
            ((LIBGS_BG *)&Bg)[0].r = 0;
            Bg = 0x51000000;
            ((LIBGS_BG *)&Bg)[0].b = 0;
            cd_read_start2(65, 0x80180000, 0x9F63);
            step_mode = step_mode + 1;
        }
        break;
    case 8:
        if (get_CD_State2() & 1) {
            set_XAsector(0x06000BC8, 0x11000CBA, 15);
            device_cmd_send(768);
            text_sys_init();
            set_kanji_string(-144, 20, 0, name_ent_load_str0, 0);
            set_kanji_string(-144, 36, 0, name_ent_load_str1, 0);
            set_kanji_string(-144, 52, 0, name_ent_load_str2, 0);
            set_kanji_string(-144, 68, 0, name_ent_load_str3, 0);
            k_disp_start(4);
            k_disp_switch(0, 0);
            k_disp_switch(1, 0);
            k_disp_switch(2, 0);
            k_disp_switch(3, 0);
            sub_sub_step_up();
        }
        break;
    default:
        break;
    }
}
