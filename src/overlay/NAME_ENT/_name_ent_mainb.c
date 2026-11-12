/* @SymbolStart: 0x80148B5C */
/* @RodataBase: 0x8014CA8C */
/* @SymbolSize: 712 */
/* @PostProc: patch_insn_reg_field 0xdc:rs:t4:v0 0xdc:rt:v0:t4 0xec:rs:t5:v0 0xec:rt:v0:t5 */
/* Address: 0x80148B64, Size: 704 bytes (176 insns) */
#include "types.h"

extern u8 step_mode;
extern u8 name_ent_serifu_no;
extern s16 cal_cw_frame;
extern s16 cal_cw_tick;
extern s32 dec_bg_handle;
extern u32 Bg;
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
extern u8 _name_ent_mainb_str0[];

extern void name_ent_serifu_set();
extern void name_ent_text_scroll();
extern s32 device_load_poll();
extern void device_port_init();
extern void cd_read_start2();
extern s32 get_CD_State2();
extern void device_cmd_send();
extern void printf();
extern void name_ent_clut_set();
extern void value_cache_find_or_load();
extern s32 dec_bg_init();
extern void indexed_byte_lsb_set();
extern void dec_bg_reset();
extern void class_clear_f();
extern void memcpy();
extern void name_ent_bg_set();
extern void sub_step_up();
extern void dec_bg_show();

void _name_ent_mainb(void)
{
    s32 sel;
    sel = step_mode;
    switch (sel) {
        case 0:
            name_ent_serifu_no = 17;
            name_ent_serifu_set();
            step_mode = step_mode + 1;
            break;
        case 1:
            name_ent_text_scroll();
            break;
        case 2:
            if (device_load_poll() & 1) {
                device_port_init(1, 0);
                cd_read_start2(49, 0x801A4000, 0xAB18);
                step_mode = step_mode + 1;
                cal_cw_mode = 5;
            }
            break;
        case 3:
            if (cal_cw_frame == 1 && cal_cw_tick == 1) {
                device_cmd_send(1540);
                printf(_name_ent_mainb_str0);
                step_mode = step_mode + 1;
            }
            break;
        case 4:
            if (get_CD_State2() & 1) {
                name_ent_clut_set();
                value_cache_find_or_load(0xAD05, 1);
                step_mode = step_mode + 1;
            }
            break;
        case 5:
            if (get_CD_State2() & 1) {
                cd_read_start2(65, 0x801D8000, 0xAC50);
                step_mode = step_mode + 1;
            }
            break;
        case 6:
            if (get_CD_State2() & 1) {
                indexed_byte_lsb_set(1, dec_bg_init(1));
                if (dec_bg_handle == -1) {
                    dec_bg_reset();
                    value_cache_find_or_load(0xAD05, 1);
                } else {
                    class_clear_f();
                    memcpy(0x80180000, 0x801D8000, 0x20800);
                    name_ent_bg_set(0x801A4000, 15);
                    Bg = 0x80000000;
                    cal_cw_prio = 10;
                    cal_cw_mode = 5;
                    cal_cw_attr = 0x41000000;
                    cal_cw_disp = 164;
                    cal_cw_img = 0x80180204;
                    cal_cw_anim_tbl = 0x80180220;
                    cal_cw_hdr = 0x801801E4;
                    cal_cw_anim = 0;
                    cal_cw_sel = 128;
                    cal_cw_x = -160;
                    cal_cw_y = -120;
                    sub_step_up();
                }
            }
            break;
    }

    dec_bg_show(1);
}
