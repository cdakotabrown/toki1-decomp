/* Function: title_data_read */
/* Address: 0x80061C6C */
/* Size: 1188 bytes (297 instructions) */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: custom_script custom_sys0w2card.py */
/* Region: Game (0x80041000-0x80085FFF) */
#include "types.h"

extern void tpage_buf_clear(void);
extern void class_clear_all(s32);
extern void dec_bg_reset(void);
extern void vram_area_clear(s32);
extern void text_sys_init(void);
extern void mes_window_switch(s32);
extern void func_80043AFC(s32);
extern void class_switch(s32);
extern void set_flag_b3_high_bit(s32);
extern void device_cmd_send(s32);
extern void func_80044AC0(s32, s32, s32, s32, s32, s32);
extern s32 device_load_poll(void);
extern void device_port_init(s32, s32);
extern void cd_read_start2(s32, s32, s32);
extern s32 get_CD_State2(void);
extern void palette_upload_enqueue(s32, s32, s32, s32, s32);
extern void load_tpage_buf_lock(s32, s32, s32);
extern void bg_set(s32 *, s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern void class_clear_n(s32);
extern void draw2d3d(s32, s32);
extern void title_init_View(void);
extern void func_800619C0(void);
extern void title_init_Coordinate(void);
extern void title_init_ModelingData(void);
extern void sub_step_up(void);
extern void memcpy(s32, s32, s32);
extern void srand(s32);

extern s32 class_max;
extern u8 back_clear_g;
extern u8 back_clear_r;
extern u8 screen_bri;
extern u8 back_clear_b;
extern u8 step_mode;
extern s32 D_800EF804;
extern u8 bg_pri;

extern u8 cal_cw_prio;
extern u8 cal_cw_mode;
extern u8 cal_cw_disp;
extern u8 cal_cw_flag4;
extern u8 cal_cw_sel;
extern s16 cal_cw_anim;
extern s16 cal_cw_x;
extern s16 cal_cw_y;
extern s32 cal_cw_img;
extern s32 cal_cw_anim_tbl;
extern s32 cal_cw_aux14;
extern s32 cal_cw_hdr;
extern s32 cal_cw_attr;

extern u8 bg0_r;
extern u8 bg0_g;
extern u8 bg0_b;

extern s16 icon_cursor_move_tbl1;
extern s32 work_fuku_1;

void title_data_read(s32 state)
{
    s32 i;
    s32 stackBuf[8];

    switch (state) {
    case 0:
        tpage_buf_clear();
        class_clear_all(0);
        dec_bg_reset();
        vram_area_clear(254);
        text_sys_init();
        mes_window_switch(0);
        func_80043AFC(1);
        class_switch(1);
        class_max = 159;
        set_flag_b3_high_bit(0);
        back_clear_g = 0;
        back_clear_r = 0;
        screen_bri = 0;
        back_clear_b = 0;
        device_cmd_send(127);
        func_80044AC0(1, 0xC5A0, 0xC58E, 0xCFB5, 0xCF99, 0xCF91);
        step_mode++;
        break;

    case 1:
        if (device_load_poll() != 1) {
            break;
        }
        device_port_init(1, 0);
        cd_read_start2(1, (s32)0x801F0000, 0xBE21);
        step_mode++;
        break;

    case 2:
        if ((get_CD_State2() & 1) == 0) {
            break;
        }
        {
            s32 base;
            base = 0x801F0800;
            do {
                memcpy(base, 0x801F0000, 2048);
                base += 2048;
            } while (base != 0x801F6800);
        }
        step_mode++;
        cd_read_start2(214, 0x80180000, 0xA0CC);
        break;

    case 3:
        if ((get_CD_State2() & 1) == 0) {
            break;
        }
        for (i = 1; i < 14; i++) {
            palette_upload_enqueue(0x801CC200, (u8)(i + 15), 1, 2, i);
        }
        palette_upload_enqueue(0x801CC200, 31, 1, 2, 15);
        palette_upload_enqueue(0x801CC200, 30, 1, 2, 14);

        {
            s32 addr;
            i = 0;
            addr = 0x80180000;
            do {
                load_tpage_buf_lock(i + 21, addr, 1);
                i++;
                addr += 0x4000;
            } while (i < 6);
            i = 0;
            addr = 0x80198000;
            do {
                load_tpage_buf_lock(i + 53, addr, 1);
                i++;
                addr += 0x4000;
            } while (i < 4);
            i = 0;
            addr = 0x801A8000;
            do {
                load_tpage_buf_lock(i + 37, addr, 1);
                i++;
                addr += 0x4000;
            } while (i < 3);
        }

        {
            s32 *p;
            s32 addr;
            addr = 0x801B4000;
            p = stackBuf;
            do {
                *p = addr;
                addr += 0x4000;
                p++;
            } while (addr < 0x801D4000);
        }

        bg_set(stackBuf, 0, 6, 3, 2, 495, -160, -120, 8, 13);
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

        bg0_g = 0;
        bg0_r = 0;
        bg_pri = 127;
        bg0_b = 0;

        func_80043AFC(0);
        srand(D_800EF804);
        draw2d3d(1, 1);
        title_init_View();
        func_800619C0();
        title_init_Coordinate();
        title_init_ModelingData();

        work_fuku_1 = (s32)icon_cursor_move_tbl1;
        {
            s16 *srcP;
            s32 *dstP;

            srcP = (&icon_cursor_move_tbl1) + 1;
            dstP = (&work_fuku_1) + 1;
            i = (s32)((&icon_cursor_move_tbl1) + 13);
            do {
                dstP[0] = srcP[0];
                dstP[1] = srcP[1];
                dstP[2] = srcP[2];
                dstP[3] = srcP[3];
                srcP += 4;
                dstP += 4;
            } while (srcP != (s16 *)i);
        }

        sub_step_up();
    }
}
