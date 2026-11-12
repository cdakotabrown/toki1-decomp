/* @SymbolStart: 0x80147B28 */
/* @RodataBase: 0x8014CA18 */
/* @PostProc: patch_insn_reg_field 0x4c:rt:a0:v1 0x50:rs:a0:v1 0x50:rt:a0:v1 0x58:rs:a0:v1 0xc8:rt:a0:v1 0xcc:rs:a0:v1 0xcc:rt:a0:v1 0xd4:rs:a0:v1 0xd4:rt:v0:a0 0xd8:rs:a0:v1 0xd8:rt:a0:v1 0xdc:rs:v0:a0 0xe0:rt:a0:v1 0x13c:rt:a0:v1 0x140:rs:a0:v1 0x140:rt:a0:v1 0x148:rt:a0:v1 0x150:rt:a0:v1 0x158:rt:a0:v1 0x160:rt:a0:v1 0x168:rs:a0:v1 0x168:rt:v0:v1 0x16c:rt:v0:v1 0x170:rs:v0:v1 */
/* @SymbolSize: 864 */
/* Address: 0x80147B30, Size: 856 bytes (214 insns) */
#include "types.h"
#include "tokimeki/libgs_types.h"

extern u8 step_mode;
extern s32 name_ent_step_cnt;
extern u8 name_ent_serifu_no;
extern u32 Bg;
extern u8 cal_cw_bright;
extern u8 cal_cw_clut;
extern u8 cal_cw_prio;
extern u8 cal_cw_mode;
extern u8 cal_cw_disp;
extern u8 cal_cw_sel;
extern u32 cal_cw_attr;
extern u32 cal_cw_img;
extern u32 cal_cw_anim_tbl;
extern u32 cal_cw_hdr;
extern u16 cal_cw_frame;
extern s16 cal_cw_x;
extern s16 cal_cw_y;

extern void name_ent_fade_box_show();
extern void name_ent_white_box_show();
extern s32 get_CD_State();
extern s32 get_CD_State2();
extern void name_ent_text_scroll();
extern void name_ent_prologue_serifu12_kuchi_ani();
extern void cd_read_start2();
extern void class_clear_n();
extern void name_ent_bg_set();
extern void set_dec_bri();
extern void class_clear_f();
extern void sub_step_up();
extern void name_ent_serifu_set();
extern void dec_bg_show();

void name_ent_prologue_serifu12_body(void)
{
    s32 sel;
    s32 a;
    s32 b;
    s32 c;
    s32 d;
    s32 cond;

    sel = step_mode;
    switch (sel) {
        case 0:
            name_ent_fade_box_show();
            a = name_ent_step_cnt + 4;
            if (a >= 256) {
                step_mode = step_mode + 1;
                name_ent_step_cnt = 0;
            } else {
                b = a / 4;
                ((LIBGS_BG *)&Bg)[0].b = b;
                ((LIBGS_BG *)&Bg)[0].g = b;
                ((LIBGS_BG *)&Bg)[0].r = b;
                cal_cw_bright = b;
                name_ent_step_cnt = a;
            }
            break;
        case 1:
            name_ent_white_box_show();
            c = name_ent_step_cnt;
            cond = c < 11;
            name_ent_step_cnt = c + 1;
            if (cond == 0) {
                cal_cw_clut = 0;
                cal_cw_sel = 128;
                cal_cw_bright = 128;
                name_ent_bg_set(0x801A4000, 15);
                step_mode = step_mode + 1;
                name_ent_step_cnt = 0;
            }
            break;
        case 2:
            name_ent_white_box_show();
            d = name_ent_step_cnt;
            ((LIBGS_BG *)&Bg)[0].b = d;
            ((LIBGS_BG *)&Bg)[0].g = d;
            ((LIBGS_BG *)&Bg)[0].r = d;
            cal_cw_bright = d;
            d = d + 8;
            name_ent_step_cnt = d;
            if (d >= 129) {
                name_ent_serifu_no = 12;
                name_ent_step_cnt = 0;
                name_ent_serifu_set();
                step_mode = step_mode + 1;
            }
            break;
        case 3:
            name_ent_white_box_show();
            if (get_CD_State() & 0x4) {
                step_mode = step_mode + 1;
            }
            break;
        case 4:
            name_ent_white_box_show();
            name_ent_text_scroll();
            name_ent_prologue_serifu12_kuchi_ani();
            break;
        case 5:
            name_ent_white_box_show();
            cd_read_start2(49, 0x801A4000, 0xAAE7);
            step_mode = step_mode + 1;
            break;
        case 6:
            name_ent_white_box_show();
            if (get_CD_State2() & 0x1) {
                Bg = 0x80000000;
                class_clear_n(96);
                cd_read_start2(25, 0x80180000, 0xAC05);
                step_mode = step_mode + 1;
            }
            break;
        case 7:
            name_ent_white_box_show();
            if (get_CD_State2() & 0x1) {
                set_dec_bri(128);
                class_clear_f();
                cal_cw_prio = 10;
                cal_cw_mode = 64;
                cal_cw_attr = 0x41000000;
                cal_cw_disp = 164;
                cal_cw_img = 0x80184288;
                cal_cw_anim_tbl = 0x801842AC;
                cal_cw_hdr = 0x80184280;
                cal_cw_frame = 0;
                cal_cw_sel = 128;
                cal_cw_x = -160;
                cal_cw_y = -120;
                name_ent_bg_set(0x801A4000, 15);
                Bg = 0x80000000;
                sub_step_up();
            }
            break;
    }

    dec_bg_show(1);
}
