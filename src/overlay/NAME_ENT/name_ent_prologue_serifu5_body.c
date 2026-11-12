/* @SymbolStart: 0x80146F1C */
/* @RodataBase: 0x8014C9B4 */
/* Address: 0x80146F24 */
/* Size: 920 bytes (230 insns) */
/* @SymbolSize: 928 */
#include "types.h"

extern u8 step_mode;
extern void name_ent_serifu_set(void);
extern s32  get_CD_State(void);
extern void name_ent_cw98_anim(void);
extern void name_ent_text_scroll(void);
extern void name_ent_kuchi_ani6(void);
extern void name_ent_cw102_set(void);
extern void device_cmd_send(s32);
extern void name_ent_cw99_set5(void);
extern void name_ent_cw100_set(void);
extern void name_ent_cw103_set(void);
extern void name_ent_cw101_set(void);
extern void value_cache_find_or_load(s32, s32);
extern s32  get_CD_State2(void);
extern void cd_read_start2(s32, s32, s32);
extern void class_clear_f(void);
extern void memcpy(s32, s32, s32);
extern void name_ent_bg_set(s32, s32);
extern s32  dec_bg_init(s32);
extern void indexed_byte_lsb_set(s32, s32);
extern void sub_step_up(void);
extern void dec_bg_show(s32);

extern u8  name_ent_serifu_no;
extern u8  step_mode;
extern s32 step_counter;
extern u8  cal_cw_prio;
extern u8  cal_cw_mode;
extern u8  cal_cw_disp;
extern u8  cal_cw_sel;
extern s16 cal_cw_frame;
extern s16 cal_cw_x;
extern s16 cal_cw_y;
extern s32 cal_cw_img;
extern s32 cal_cw_anim_tbl;
extern s32 cal_cw_hdr;
extern s32 cal_cw_attr;

void name_ent_prologue_serifu5_body(void) {
    s32 idx;
    s32 n;

    idx = step_mode;
    switch (idx) {
    case 0:
        name_ent_serifu_no = 5;
        name_ent_serifu_set();
        step_mode++;
        break;
    case 1:
        if (get_CD_State() & 4) {
            step_mode++;
        }
        break;
    case 2:
        name_ent_cw98_anim();
        name_ent_text_scroll();
        name_ent_kuchi_ani6();
        if (step_counter == 4) {
            name_ent_cw102_set();
        }
        break;
    case 3:
        name_ent_serifu_no = 6;
        name_ent_serifu_set();
        step_mode++;
        break;
    case 4:
        name_ent_text_scroll();
        name_ent_kuchi_ani6();
        break;
    case 5:
        name_ent_serifu_no = 7;
        name_ent_serifu_set();
        step_mode++;
        break;
    case 6:
        name_ent_text_scroll();
        name_ent_kuchi_ani6();
        break;
    case 7:
        name_ent_serifu_no = 8;
        name_ent_serifu_set();
        step_mode++;
        break;
    case 8:
        name_ent_text_scroll();
        break;
    case 9:
        device_cmd_send(1283);
        name_ent_cw99_set5();
        name_ent_serifu_no = 9;
        name_ent_serifu_set();
        step_mode++;
        break;
    case 10:
        name_ent_text_scroll();
        name_ent_cw100_set();
        break;
    case 11:
        name_ent_cw103_set();
        name_ent_serifu_no = 10;
        name_ent_serifu_set();
        step_mode++;
        break;
    case 12:
        name_ent_text_scroll();
        name_ent_cw101_set();
        break;
    case 13:
        value_cache_find_or_load(0xACEC, 1);
        step_mode++;
        break;
    case 14:
        if (get_CD_State2() & 1) {
            cd_read_start2(49, 0x801A4000, 0xAA85);
            step_mode++;
        }
        break;
    case 15:
        if (get_CD_State2() & 1) {
            cd_read_start2(25, 0x801F0000, 0xABE3);
            step_mode++;
        }
        break;
    case 16:
        if (get_CD_State2() & 1) {
            class_clear_f();
            memcpy(0x80180000, 0x801F0000, 0xC800);
            name_ent_bg_set(0x801A4000, 15);
            n = dec_bg_init(1);
            indexed_byte_lsb_set(1, n);
            cal_cw_prio = 10;
            cal_cw_mode = 64;
            cal_cw_attr = 0x41000000;
            cal_cw_disp = 164;
            cal_cw_img = 0x8018014C;
            cal_cw_anim_tbl = 0x80180170;
            cal_cw_hdr = 0x80180140;
            cal_cw_frame = 0;
            cal_cw_sel = 128;
            cal_cw_x = -224;
            cal_cw_y = -120;
            sub_step_up();
        }
        break;
    }
    dec_bg_show(1);
}
