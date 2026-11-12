/* @CompileEnv: IDO52_FORCE_WEB_COLOR=0:v1 */
/* @SymbolStart: 0x80149774 */
/* Address: 0x8014977C */
/* Size: 660 bytes (165 insns) */
/* @SymbolSize: 668 */
/* @RodataBase: 0x8014CABC */
#include "types.h"
#include "tokimeki/class_w.h"

extern u8 step_mode;
extern void name_ent_serifu_set(void);
extern void name_ent_text_scroll(void);
extern void name_ent_kuchi_anie(void);
extern void name_ent_slots_reset(void);
extern void value_cache_find_or_load(s32, s32);
extern s32  get_CD_State2(void);
extern void cd_read_start2(s32, s32, s32);
extern s32  dec_bg_init(s32);
extern void indexed_byte_lsb_set(s32, s32);
extern void dec_bg_reset(void);
extern void class_clear_f(void);
extern void name_ent_bg_set(s32, s32);
extern void memcpy(s32, s32, s32);
extern void sub_step_up(void);
extern s32  rand(void);
extern void name_ent_cw98_sete(void);
extern void dec_bg_show(s32);

extern u8  name_ent_serifu_no;
extern u8  step_mode;
extern s32 dec_bg_handle;
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
extern CLASS_W class_w[];

void _name_ent_maine(void) {
    s32 n;

    switch (step_mode) {
    case 0:
        name_ent_serifu_no = 19;
        name_ent_serifu_set();
        step_mode++;
        break;
    case 1:
        name_ent_text_scroll();
        name_ent_kuchi_anie();
        break;
    case 2:
        name_ent_slots_reset();
        step_mode++;
        break;
    case 3:
        if (class_w[99].anim_flag & 1) {
            break;
        }
        step_mode++;
        value_cache_find_or_load(0xACEC, 1);
        break;
    case 4:
        if (get_CD_State2() & 1) {
            cd_read_start2(49, 0x801A4000, 0xAB7A);
            step_mode++;
        }
        break;
    case 5:
        if (get_CD_State2() & 1) {
            cd_read_start2(25, 0x801E8000, 0xACBB);
            step_mode++;
        }
        break;
    case 6:
        if (get_CD_State2() & 1) {
            n = dec_bg_init(1);
            indexed_byte_lsb_set(1, n);
            if (dec_bg_handle == -1) {
                dec_bg_reset();
                value_cache_find_or_load(0xACEC, 1);
            } else {
                class_clear_f();
                name_ent_bg_set(0x801A4000, 15);
                memcpy(0x80180000, 0x801E8000, 0xC800);
                cal_cw_prio = 11;
                cal_cw_mode = 64;
                cal_cw_attr = 0x41000000;
                cal_cw_disp = 164;
                cal_cw_img = 0x80180258;
                cal_cw_anim_tbl = 0x80180298;
                cal_cw_hdr = 0x8018024C;
                cal_cw_frame = 0;
                cal_cw_sel = 128;
                cal_cw_x = -240;
                cal_cw_y = -120;
                sub_step_up();
            }
        }
        break;
    }
    if ((rand() & 0xFF) == 255) {
        if ((class_w[98].anim_flag & 1) == 0) {
            name_ent_cw98_sete();
        }
    }
    dec_bg_show(1);
}
