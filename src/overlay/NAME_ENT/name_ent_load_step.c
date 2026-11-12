/* @SymbolStart: 0x801461F0 */
/* @RodataBase: 0x8014C988 */
/* Address: 0x801461F8 */
/* Size: 828 bytes (207 insns) */
/* @SymbolSize: 836 */
#include "types.h"

extern u8 step_mode;
extern void class_clear_n(s32);
extern void cd_read_start2(s32, s32, s32);
extern void name_ent_bg_set(s32, s32);
extern void name_ent_serifu_set(void);
extern s32  get_CD_State2(void);
extern s32  rand(void);
extern void name_ent_text_scroll(void);
extern void name_ent_clut_set(void);
extern s32  value_cache_find_or_load(s32, s32);
extern void indexed_byte_lsb_set(s32, s32);
extern void memcpy(s32, s32, s32);
extern void k_sub_reset(void);
extern void sub_step_up(void);
extern void dec_bg_show(s32);

extern u8  name_ent_serifu_no;
extern s32 name_ent_step_cnt;
extern u8  step_mode;
extern u8  cal_cw_prio;
extern u8  cal_cw_mode;
extern u8  cal_cw_disp;
extern u8  cal_cw_sel;
extern s16 cal_cw_anim;
extern s16 cal_cw_x;
extern s16 cal_cw_y;
extern s32 cal_cw_img;
extern s32 cal_cw_anim_tbl;
extern s32 cal_cw_hdr;
extern s32 cal_cw_attr;
extern u8  cw97_prio;
extern u8  cw97_mode;
extern u8  cw97_disp;
extern u8  cw97_sel;
extern s16 cw97_anim;
extern s16 cw97_x;
extern s16 cw97_y;
extern s32 cw97_img;
extern s32 cw97_anim_tbl;
extern s32 cw97_hdr;
extern s32 cw97_attr;

void name_ent_load_step(void) {
    s32 idx;
    s32 n;

    idx = step_mode;
    switch (idx) {
    case 0:
        class_clear_n(96);
        cd_read_start2(9, 0x80180000, 0xABB8);
        name_ent_serifu_no = 4;
        name_ent_bg_set(0x801A4000, 15);
        name_ent_serifu_set();
        step_mode++;
        break;
    case 1:
        if (get_CD_State2() & 1) {
            cal_cw_prio = 10;
            cal_cw_mode = 0;
            cal_cw_attr = 0x41000000;
            cal_cw_disp = 172;
            cal_cw_img = 0x8018007C;
            cal_cw_anim_tbl = 0x8018008C;
            cal_cw_hdr = 0x80180078;
            cal_cw_anim = 0;
            cal_cw_sel = 128;
            cal_cw_x = -160;
            cal_cw_y = -120;
            step_mode++;
        }
        break;
    case 2:
        if ((rand() & 0x7F) == 127 && (cal_cw_mode & 1) == 0) {
            cal_cw_mode = 17;
        }
        name_ent_text_scroll();
        break;
    case 3:
        cd_read_start2(49, 0x801A4000, 0xAB18);
        step_mode++;
        break;
    case 4:
        if (get_CD_State2() & 1) {
            name_ent_clut_set();
            n = value_cache_find_or_load(0xACDD, 1);
            indexed_byte_lsb_set(1, n);
            memcpy(0x80180000, 0x801F0000, 0x8800);
            name_ent_bg_set(0x801A4000, 14);
            class_clear_n(96);
            cal_cw_prio = 10;
            cal_cw_mode = 0;
            cal_cw_attr = 0x41000000;
            cal_cw_disp = 172;
            cal_cw_img = 0x801800D8;
            cal_cw_anim_tbl = 0x801800F0;
            cal_cw_hdr = 0x801800D4;
            cal_cw_anim = 0;
            cal_cw_sel = 128;
            cal_cw_x = -160;
            cal_cw_y = -120;
            class_clear_n(97);
            cw97_prio = 10;
            cw97_mode = 64;
            cw97_attr = 0x41000000;
            cw97_disp = 172;
            cw97_img = 0x80184184;
            cw97_anim_tbl = 0x8018418C;
            cw97_hdr = 0x80184180;
            cw97_anim = 0;
            cw97_sel = 128;
            cw97_x = -32;
            cw97_y = -108;
            k_sub_reset();
            name_ent_step_cnt = 0;
            sub_step_up();
        }
        break;
    }
    dec_bg_show(1);
}
