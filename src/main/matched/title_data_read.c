/* Address: 0x80061C64 (the .sym / o.bin entry 0x80061C6C is the prologue; */
/* Size: 1196 bytes (299 instructions) */
/* Region: Game (IDO 5.2) */
/* @SymbolStart: 0x80061C64 */
/* @SymbolSize: 1196 */
/* @CompileFlags: -Wc,-pic1 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=2:v1 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"
#include "tokimeki/libgs_types.h"

extern u8 step_mode;
extern s32 class_max;
extern u8 back_clear_r;
extern u8 back_clear_g;
extern u8 back_clear_b;
extern u8 screen_bri;
extern u8 cal_cw_prio;
extern u8 cal_cw_mode;
extern u8 cal_cw_disp;
extern u8 cal_cw_flag4;
extern u8 cal_cw_sel;
extern s32 cal_cw_img;
extern s32 cal_cw_anim_tbl;
extern s16 cal_cw_anim;
extern s16 cal_cw_x;
extern s16 cal_cw_y;
extern s32 cal_cw_hdr;
extern s32 cal_cw_attr;
extern u8 bg_pri;
extern s32 vblnk_counter;
extern s16 title_data_read_tbl0[13];
extern s32 work_fuku_1[13];
extern LIBGS_BG Bg[];

extern void tpage_buf_clear(void);
extern void class_clear_all(s32);
extern void dec_bg_reset(void);
extern void vram_area_clear(s32);
extern void text_sys_init(void);
extern void mes_window_switch(s32);
extern void back_clear_switch(s32);
extern void class_switch(s32);
extern void set_flag_b3_high_bit(s32);
extern s32 device_cmd_send(s32);
extern void device_load_start(s32, s32, s32, s32, s32, s32);
extern s32 device_load_poll(void);
extern s32 device_port_init(s32, s32);
extern void cd_read_start2(u8, s32, s32);
extern u8 get_CD_State2(void);
extern void memcpy(u8 *, u8 *, s32);
extern s32 palette_upload_enqueue(u32, u8, u8, u8, u8);
extern void load_tpage_buf_lock(s32, s32, s32);
extern void bg_set(s32 *, s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern void class_clear_n(s32);
extern void srand(s32);
extern void draw2d3d(s32, s32);
extern void title_init_View(void);
extern void title_init_Light(void);
extern void title_init_Coordinate(void);
extern void title_init_ModelingData(void);
extern s32 sub_step_up(void);

void title_data_read(void)
{
    u32 step;
    s16 *src;
    s32 slots[8];
    s32 i;
    s32 addr;
    u8 *p;

    step = step_mode;
    switch (step) {
    case 0:
        tpage_buf_clear();
        class_clear_all(0);
        dec_bg_reset();
        vram_area_clear(254);
        text_sys_init();
        mes_window_switch(0);
        back_clear_switch(1);
        class_switch(1);
        class_max = 159;
        set_flag_b3_high_bit(0);
        back_clear_g = 0;
        back_clear_r = 0;
        screen_bri = 0;
        back_clear_b = 0;
        device_cmd_send(127);
        device_load_start(1, 0xC5A0, 0xC58E, 0xCFB5, 0xCF99, 0xCF91);
        step = step_mode;
        step_mode = step + 1;
        break;
    case 1:
        if (device_load_poll() == 1) {
            device_port_init(1, 0);
            cd_read_start2(1, 0x801F0000, 0xBE21);
            step = step_mode;
        step_mode = step + 1;
        }
        break;
    case 2:
        if (get_CD_State2() & 1) {
            for (p = (u8 *)0x801F0800; p != (u8 *)0x801F6800; p += 2048) {
                memcpy(p, (u8 *)0x801F0000, 2048);
            }
            step = step_mode;
        step_mode = step + 1;
            cd_read_start2(214, 0x80180000, 0xA0CC);
        }
        break;
    case 3:
        if (get_CD_State2() & 1) {
            for (i = 1; i < 14; i++) {
                palette_upload_enqueue(0x801CC200, i + 15, 1, 2, i);
            }
            palette_upload_enqueue(0x801CC200, 31, 1, 2, 15);
            palette_upload_enqueue(0x801CC200, 30, 1, 2, 14);
            i = 0;
            addr = 0x80180000;
            for (; i < 6; i++) {
                load_tpage_buf_lock(i + 21, addr, 1);
                addr += 0x4000;
            }
            addr = 0x80198000;
            for (i = 0; i < 4; i++) {
                load_tpage_buf_lock(i + 53, addr, 1);
                addr += 0x4000;
            }
            addr = 0x801A8000;
            for (i = 0; i < 3; i++) {
                load_tpage_buf_lock(i + 37, addr, 1);
                addr += 0x4000;
            }
            for (i = 0; i < 8; i++) {
                slots[i] = (s32)0x801B4000 + i * 0x4000;
            }
            bg_set(slots, 0, 6, 3, 2, 495, -160, -120, 8, 13);
            palette_upload_enqueue(0x801CC000, 15, 1, 3, 0);
            class_clear_n(96);
            cal_cw_prio = 13;
            cal_cw_mode = 1;
            cal_cw_attr = 0x41000000;
            cal_cw_disp = 164;
            cal_cw_img = 0x801EAB3C;
            cal_cw_anim_tbl = 0x801EAB6C;
            cal_cw_hdr = 0x801EAB20;
            cal_cw_anim = 0;
            cal_cw_flag4 = 8;
            cal_cw_sel = 128;
            cal_cw_x = -160;
            cal_cw_y = -120;
            Bg[0].g = 0;
            Bg[0].r = 0;
            bg_pri = 127;
            Bg[0].b = 0;
            back_clear_switch(0);
            srand(vblnk_counter);
            draw2d3d(1, 1);
            title_init_View();
            title_init_Light();
            title_init_Coordinate();
            title_init_ModelingData();
            work_fuku_1[0] = title_data_read_tbl0[0];
            src = &title_data_read_tbl0[1];
            for (i = 0; i < 12; i++) {
                work_fuku_1[i + 1] = src[i];
            }
            sub_step_up();
        }
        break;
    }
}
