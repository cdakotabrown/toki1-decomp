/* @SymbolStart: 0x80144F18 */
/* @SymbolSize: 924 */
/* name_ent_prologue_init - NAME_ENT overlay, Address: 0x80144F28, Size: 908 bytes */
/* HEAD ABSORPTION: `@SymbolStart: 0x80144F18` widens the compare window over */
/* 2. `@CompileEnv: IDO52_CONST_NOINREG_VALUES=1` -- without it IDO parks the */
/* @PostProc: swap_instructions --offset1 0x2a4 --offset2 0x2a8 */
/* @PostProc: patch_insn_reg_field 0x18:rt:v0:v1 0x1c:rs:v0:v1 0x1c:rt:v0:v1 0x34:rs:v0:v1 0x138:rs:v0:v1 0x184:rs:v0:v1 0x190:rs:v0:v1 0x190:rt:t5:t3 0x1a4:rt:t5:t3 0x1ac:rs:v0:v1 0x234:rs:v0:v1 0x110:rt:t0:v1 0x114:rs:t0:v1 0x114:rt:t0:v1 0x11c:rs:t0:v1 0x11c:rt:t1:t0 0x120:rt:t1:t0 0x168:rt:t2:v1 0x16c:rs:t2:v1 0x16c:rt:t2:v1 0x174:rs:t2:v1 0x174:rt:t3:t1 0x17c:rt:t3:t1 0x188:rt:t4:t2 0x18c:rs:t4:t2 0x18c:rt:t4:t2 0x194:rs:t4:t2 0x1bc:rt:t6:t4 0x1c0:rs:t6:t4 0x1dc:rt:t7:t5 0x1e0:rs:t7:t5 0x1e0:rt:t7:t5 0x1e8:rs:t7:t5 0x218:rt:t8:v1 0x21c:rs:t8:v1 0x21c:rt:t8:v1 0x224:rs:t8:v1 0x224:rt:t9:t6 0x22c:rt:t9:t6 0x244:rt:t0:t7 0x248:rs:t0:t7 0x258:rt:t1:t8 0x260:rt:t1:t8 0x268:rt:t2:t9 0x26c:rt:t2:t9 0x274:rt:t3:t0 0x278:rt:t3:t0 0x280:rt:t4:t1 0x284:rt:t4:t1 0x288:rt:t5:t2 0x28c:rs:t5:t2 0x28c:rt:t5:t2 0x294:rt:t5:t2 0x298:rt:t6:t3 0x29c:rs:t6:t3 0x29c:rt:t6:t3 0x2a4:rt:t6:t3 0x2a8:rt:v0:t4 0x2ac:rs:v0:t4 0x2ac:rt:v0:t4 0x2b4:rt:v0:t4 0x2bc:rt:t7:t5 0x2c0:rt:t7:t5 0x2d0:rt:t8:t6 0x2d4:rt:t8:t6 0x2dc:rt:t9:t7 0x2e0:rt:t9:t7 0x2e8:rt:t0:t8 0x2ec:rt:t0:t8 0x2f4:rt:t1:t9 0x2f8:rt:t1:t9 0x2fc:rs:v0:t4 0x314:rt:t2:t1 0x320:rs:t2:t1 0x390:rt:t3:t2 0x398:rt:t3:t2 */
/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=1 */
#include "types.h"

extern void draw2d3d(s32, s32);
extern void class_switch(s32);
extern void vram_area_clear(s32);
extern void prim_init(void);
extern void class_clear_f(void);
extern void back_clear_switch(s32);
extern void tpage_buf_clear(void);
extern void srand(s32);
extern void text_sys_init(void);
extern void k_speed_set(s32);
extern void basyo_init(s32);
extern void k_disp_start(s32);
extern void k_sub_reset_point_set(void);
extern void parameter_show_init(void);
extern void parameter_disp_switch(s32);
extern void hizuke_init(void);
extern void message_window_init(void);
extern void dec_bg_reset(void);
extern void hizuke_disp_switch(s32);
extern s32  device_load_poll(void);
extern void device_port_init(s32, s32);
extern void value_cache_find_or_load(s32, s32);
extern s32  get_CD_State2(void);
extern s32  dec_bg_init(s32);
extern void indexed_byte_lsb_set(s32, s32);
extern void cd_read_start2(s32, s32, s32);
extern void class_clear_n(s32);
extern void load_tpage_buf_lock(s32, s32, s32);
extern void palette_upload_enqueue(s32, s32, s32, s32, s32);
extern void clut_gray_set(s32, s32);
extern void sub_sub_step_up(void);

extern s32 step_counter;
extern u8  step_mode;
extern u8  cursor_cw_disp;
extern u8  anim_fade;
extern u8  name_ent_serifu_no;
extern volatile u8 vblank_flag;
extern volatile s32 frame_counter;
extern u8  name_ent_prologue_init_str0[];
extern u8  screen_bri;
extern s32 dec_bg_handle;
extern u8  cursor_work_bak[];

extern u8  cal_cw_prio;
extern u8  cal_cw_mode;
extern u8  cal_cw_disp;
extern u8  cal_cw_sel;
extern u8  cal_cw_rate;
extern u8  cal_cw_bright;
extern s32 cal_cw_img;
extern s32 cal_cw_anim_tbl;
extern s16 cal_cw_x;
extern s16 cal_cw_y;
extern s32 *cal_cw_hdr;
extern s32 cal_cw_attr;
extern u8  cal_cw_clut;

void name_ent_prologue_init(void)
{
    s32 i;

    step_counter = step_counter + 1;
    if (step_mode == 0) {
        cursor_cw_disp = 0;
        anim_fade = 254;
        name_ent_serifu_no = 255;
        draw2d3d(1, 0);
        class_switch(1);
        vram_area_clear(254);
        prim_init();
        class_clear_f();
        back_clear_switch(1);
        tpage_buf_clear();
        vblank_flag = 0;
        srand(frame_counter);
        text_sys_init();
        k_speed_set(5);
        basyo_init((s32)&name_ent_prologue_init_str0);
        k_disp_start(1);
        k_sub_reset_point_set();
        parameter_show_init();
        parameter_show_init();
        parameter_disp_switch(0);
        hizuke_init();
        message_window_init();
        screen_bri = 0;
        dec_bg_reset();
        step_mode = (u8)(step_mode + 1);
        hizuke_disp_switch(1);
    } else if (step_mode == 1) {
        if (device_load_poll() == 1) {
            device_port_init(0, 0);
            value_cache_find_or_load(18763, 1);
            step_mode = (u8)(step_mode + 1);
        }
    } else if (step_mode == 2) {
        if ((u32)step_counter >= 17U) {
            step_mode = (u8)(step_mode + 1);
        }
    } else if (step_mode == 3) {
        if ((get_CD_State2() & 1) != 0) {
            indexed_byte_lsb_set(1, dec_bg_init(1));
            if (dec_bg_handle == -1) {
                dec_bg_reset();
                value_cache_find_or_load(18763, 1);
            } else {
                cd_read_start2(13, 0x80180000, 0xABAB);
                step_mode = (u8)(step_mode + 1);
            }
        }
    } else if (step_mode == 4) {
        if ((get_CD_State2() & 1) != 0) {
            class_clear_n(96);
            cal_cw_prio = 10;
            cal_cw_mode = 1;
            cal_cw_attr = 0x41000000;
            cal_cw_disp = 36;
            cal_cw_img = 0x801800AC;
            cal_cw_anim_tbl = 0x801800C0;
            cal_cw_hdr = (s32 *)0x801800A8;
            cal_cw_rate = 2;
            cal_cw_clut = 0;
            cal_cw_sel = 128;
            cal_cw_x = -160;
            cal_cw_y = -120;
            cal_cw_bright = 128;
            load_tpage_buf_lock(26, *cal_cw_hdr, 1);
            i = 0;
            do {
                palette_upload_enqueue(0x80184490, (i + 16) & 0xFF, 1, 2, i);
                i++;
            } while (i != 16);
            clut_gray_set(0x80184610, (s32)&cursor_work_bak);
            palette_upload_enqueue((s32)&cursor_work_bak, 13, 1, 2, 0);
            sub_sub_step_up();
        }
    }
    if ((u32)screen_bri < 128U) {
        screen_bri = (u8)(screen_bri + 1);
    }
}
