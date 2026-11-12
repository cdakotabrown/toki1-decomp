/* @SymbolStart: 0x80149288 */
/* @RodataBase: 0x8014CAA8 */
/* @SymbolSize: 492 */
/* Address: 0x80149290, Size: 484 bytes (121 insns) */
#include "types.h"
#include "tokimeki/class_w.h"

extern u8 step_mode;
extern u8 name_ent_serifu_no;
extern u32 Bg;
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
extern CLASS_W class_w[];

extern void name_ent_serifu_set();
extern s32 get_CD_State();
extern void name_ent_text_scroll();
extern void name_ent_kuchi_anid();
extern void cd_read_start2();
extern s32 get_CD_State2();
extern void class_clear_f();
extern void indexed_byte_toggle();
extern void memcpy();
extern void sub_step_up();
extern s32 rand();
extern void name_ent_cw98_setd();
extern void dec_bg_show();

void _name_ent_maind(void)
{
    s32 sel;
    sel = step_mode;
    switch (sel) {
        case 0:
            name_ent_serifu_no = 18;
            name_ent_serifu_set();
            step_mode = step_mode + 1;
            break;
        case 1:
            if (get_CD_State() & 0x4) {
                step_mode = step_mode + 1;
            }
            break;
        case 2:
            name_ent_text_scroll();
            name_ent_kuchi_anid();
            break;
        case 3:
            cd_read_start2(33, 0x801E8000, 0xAC9A);
            step_mode = step_mode + 1;
            break;
        case 4:
            if (get_CD_State2() & 0x1) {
                class_clear_f();
                indexed_byte_toggle(1);
                Bg = 0x80000000;
                memcpy(0x80180000, 0x801E8000, 0x10800);
                cal_cw_prio = 9;
                cal_cw_mode = 64;
                cal_cw_attr = 0x41000000;
                cal_cw_disp = 164;
                cal_cw_img = 0x80180294;
                cal_cw_anim_tbl = 0x801802C8;
                cal_cw_hdr = 0x80180284;
                cal_cw_frame = 0;
                cal_cw_sel = 128;
                cal_cw_x = -160;
                cal_cw_y = -120;
                sub_step_up();
            }
            break;
    }

    if ((rand() & 0xff) == 255) {
        if (!(class_w[98].anim_flag & 1)) {
            name_ent_cw98_setd();
        }
    }
    dec_bg_show(1);
}
