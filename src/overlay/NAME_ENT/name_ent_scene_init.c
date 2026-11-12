/* @SymbolStart: 0x8013BCB0 */
/* @SymbolSize: 756 */
/* @PostProc: swap_instructions --offset1 0xA4 --offset2 0xA8 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x28:rs:v0:v1 0xb4:rt:v0:a1 0xb8:rs:v0:a1 0xbc:rt:at:a0 0xc0:rt:a0:at 0xd0:rt:v0:a1 0xd4:rs:v0:a1 0xd8:rt:at:a0 0xdc:rt:a0:at 0xec:rt:v0:a1 0xf0:rs:v0:a1 0xf4:rt:at:a0 0xf8:rt:a0:at 0x108:rt:v0:a1 0x10c:rs:v0:a1 0x110:rt:at:a0 0x114:rt:a0:at 0x124:rt:v0:a1 0x128:rs:v0:a1 0x12c:rt:at:a0 0x130:rt:a0:at 0x140:rt:v0:a1 0x144:rs:v0:a1 0x148:rt:at:a0 0x14c:rt:a0:at 0x15c:rt:v0:a1 0x160:rs:v0:a1 0x164:rt:at:a0 0x168:rt:a0:at 0x2c0:rt:t8:v1 0x2c4:rs:t8:v1 0x2c4:rt:t8:v1 0x2cc:rs:t8:v1 0x2cc:rt:t9:t8 0x2d4:rt:t9:t8 */
#include "types.h"

extern u8 return_schedule_tbl0;
extern u8 screen_bri;
extern u8 voice_level;
extern u8 icon_mode;
extern u8 csr_speed;
extern u8 click_mode;
extern u8 click_speed;
extern u8 mouse_btn_mode;
extern u8 vblank_flag;
extern s32 class_max;
extern u8 menu_mode;
extern u8 step_mode;
extern u8 option_on;
extern u8 cursor_work_bak[];
extern s32 album_work;
extern s32 work_fuku;

extern u8 name_ent_optset_fmt[];
extern u8 name_ent_optset_kspeed_fmt[];
extern u8 name_ent_optset_icon_fmt[];
extern u8 name_ent_optset_menu_fmt[];
extern u8 name_ent_optset_csr_fmt[];
extern u8 name_ent_optset_sound_fmt[];
extern u8 name_ent_optset_click_fmt[];
extern u8 name_ent_optset_clkspd_fmt[];
extern u8 name_ent_optset_mouse_fmt[];

extern u8 name_ent_entry_mode;
extern s32 name_ent_menu_lock;
extern s32 name_ent_keep_opt_on;
extern u8 name_ent_keep_k_speed;
extern u8 name_ent_keep_icon_mode;
extern u8 name_ent_keep_menu_mode;
extern u8 name_ent_keep_csr_speed;
extern u8 name_ent_keep_voice_level;
extern u8 name_ent_keep_click_mode;
extern u8 name_ent_keep_click_speed;
extern u8 name_ent_keep_mouse_btn;
extern s32 name_ent_iris_size;
extern s32 name_ent_iris_x;
extern s32 name_ent_iris_y;
extern s32 name_ent_iris_vx;
extern s32 name_ent_iris_vy;

extern void prim_init(void);
extern void draw2d3d(s32, s32);
extern void back_clear_switch(s32);
extern void palette_upload_enqueue();
extern void device_load_start();
extern void sub_sub_step_up(void);
extern s32 device_load_poll(void);
extern void make_fan(s32, s32, s32);
extern void tpage_buf_clear(void);
extern void class_switch(s32);
extern void class_clear_f(void);
extern void make_color_bar16();
extern void text_sys_init(void);
extern void kanji_cell_width_set(s32);
extern void k_speed_set(s32);
extern s32 get_k_speed(void);
extern void mes_window_switch(s32);
extern void mascot_disp_switch(s32);
extern void cursor_init(s32);
extern void set_flag_b3_high_bit(s32);
extern void printf();
extern void srand(s32);

void name_ent_scene_init(void)
{
    s32 v;

    switch (step_mode) {
    case 0:
        return_schedule_tbl0 = 0;
        draw2d3d(1, 0);
        class_switch(1);
        prim_init();
        class_max = 160;
        class_clear_f();
        if (option_on != 0) {
            printf(name_ent_optset_fmt);
            name_ent_keep_opt_on = option_on;
            v = get_k_speed();
            name_ent_keep_k_speed = v;
            printf(name_ent_optset_kspeed_fmt, v);
            name_ent_keep_icon_mode = icon_mode;
            printf(name_ent_optset_icon_fmt, icon_mode);
            name_ent_keep_menu_mode = menu_mode;
            printf(name_ent_optset_menu_fmt, menu_mode);
            name_ent_keep_csr_speed = csr_speed;
            printf(name_ent_optset_csr_fmt, csr_speed);
            name_ent_keep_voice_level = voice_level;
            printf(name_ent_optset_sound_fmt, voice_level);
            name_ent_keep_click_mode = click_mode;
            printf(name_ent_optset_click_fmt, click_mode);
            name_ent_keep_click_speed = click_speed;
            printf(name_ent_optset_clkspd_fmt, click_speed);
            name_ent_keep_mouse_btn = mouse_btn_mode;
            printf(name_ent_optset_mouse_fmt, mouse_btn_mode);
        } else {
            name_ent_keep_opt_on = 0;
        }
        option_on = 0;
        name_ent_menu_lock = 1;
        name_ent_entry_mode = 0;
        srand(album_work);
        k_speed_set(1);
        back_clear_switch(1);
        mascot_disp_switch(0);
        tpage_buf_clear();
        vblank_flag = 0;
        mes_window_switch(0);
        text_sys_init();
        kanji_cell_width_set(14);
        screen_bri = 0;
        work_fuku = 255;
        cursor_init(0);
        set_flag_b3_high_bit(0);
        name_ent_iris_size = 1;
        name_ent_iris_x = -80;
        name_ent_iris_y = -80;
        name_ent_iris_vx = 8;
        name_ent_iris_vy = 8;
        make_color_bar16(cursor_work_bak, 0xDF7FCF, 0xDF7FCF, 16, 0);
        palette_upload_enqueue(cursor_work_bak, 23, 1, 1, 0);
        make_fan(0x80180000, 26, 0);
        device_load_start(0, 0xBFA6, 0xBF87, 0xCD2F, 0xCCE7, 0xCCDF);
        step_mode = step_mode + 1;
        break;
    case 1:
        if (device_load_poll() == 1) {
            sub_sub_step_up();
        }
        break;
    }
}
