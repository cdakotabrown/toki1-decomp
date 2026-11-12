/* @SymbolStart: 0x80148818 */
/* @SymbolSize: 792 */
/* @RodataBase: 0x8014CA68 */
/* @CompileEnv: IDO52_COPYPROP_SITE=lod:3:deny,lod:4:deny,lod:5:deny IDO52_FORCE_WEB_COLOR=59:v1,65:v0 */
/* Address: 0x80148820, Size: 784 bytes (196 insns) */
#include "types.h"

extern u8 step_mode;
extern u8 name_ent_serifu_no;
extern s32 dec_bg_handle;
extern u8 cal_cw_prio;
extern u8 cal_cw_mode;
extern u8 cal_cw_disp;
extern u8 cal_cw_sel;
extern u32 cal_cw_attr;
extern u32 cal_cw_img;
extern u32 cal_cw_anim_tbl;
extern u32 cal_cw_hdr;
extern u16 cal_cw_anim;
extern s16 cal_cw_x;
extern s16 cal_cw_y;
extern s16 cw97_x;
extern s16 cw98_x;
extern s32 class_w[];

extern void name_ent_kuchi_ania();
extern s32 get_CD_State();
extern void name_ent_text_scroll();
extern void name_ent_serifu_set();
extern void cd_read_start2();
extern s32 get_CD_State2();
extern void name_ent_clut_set();
extern void value_cache_find_or_load();
extern void indexed_byte_lsb_set();
extern s32 dec_bg_init();
extern void dec_bg_reset();
extern void class_clear_f();
extern void set_dec_bri();
extern void memcpy();
extern void name_ent_bg_set();
extern void device_cmd_send();
extern void device_load_start();
extern void sub_step_up();
extern void dec_bg_show();

void _name_ent_maina(void)
{
    s32 sel;
    s32 *base;
    s32 hw;

    sel = step_mode;
    switch (sel) {
        case 0:
            name_ent_serifu_no = 15;
            name_ent_serifu_set();
            step_mode = step_mode + 1;
            break;
        case 1:
            if (get_CD_State() & 4) {
                step_mode = step_mode + 1;
            }
            break;
        case 2:
            name_ent_kuchi_ania();
            name_ent_text_scroll();
            break;
        case 3:
            name_ent_serifu_no = 16;
            name_ent_serifu_set();
            step_mode = step_mode + 1;
            break;
        case 4:
            name_ent_text_scroll();
            break;
        case 5:
            cd_read_start2(49, 0x801A4000, 0xAB18);
            step_mode = step_mode + 1;
            break;
        case 6:
            if (get_CD_State2() & 1) {
                name_ent_clut_set();
                cd_read_start2(17, 0x801F0000, 0xAC3F);
                step_mode = step_mode + 1;
            }
            break;
        case 7:
            if (get_CD_State2() & 1) {
                value_cache_find_or_load(0xACF6, 1);
                step_mode = step_mode + 1;
            }
            break;
        case 8:
            if (get_CD_State2() & 1) {
                indexed_byte_lsb_set(1, dec_bg_init(1));
                if (dec_bg_handle == -1) {
                    dec_bg_reset();
                    value_cache_find_or_load(0xACF6, 1);
                } else {
                    class_clear_f();
                    set_dec_bri(128);
                    memcpy(0x80180000, 0x801F0000, 0x8800);
                    name_ent_bg_set(0x801A4000, 15);
                    cal_cw_prio = 10;
                    cal_cw_mode = 0;
                    cal_cw_attr = 0x41000000;
                    cal_cw_disp = 164;
                    cal_cw_img = 0x801800D8;
                    cal_cw_anim_tbl = 0x801800F0;
                    cal_cw_hdr = 0x801800D4;
                    cal_cw_anim = 0;
                    cal_cw_sel = 128;
                    cal_cw_x = -160;
                    cal_cw_y = -120;
                    device_cmd_send(5);
                    device_cmd_send(193);
                    device_load_start(1, 0, 0, 0xCBE2, 0xCBB8, 0xCBB3);
                    sub_step_up();
                }
            }
            break;
    }

    if ((u32)step_mode >= 2) {
        base = class_w;
        base[1641] -= 32768;
        hw = ((s16 *)base)[3283];
        cw97_x = hw;
        cw98_x = hw;
    }
    dec_bg_show(1);
}
