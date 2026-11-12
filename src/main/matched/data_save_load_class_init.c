/* Likely Symbol: none (a proximity o.bin name was retracted 2026-10-09) */
/* Address: 0x800696C0 */
/* Size: 200 bytes */
/* Compiler: IDO 5.2 */
/* Region: GCC (0x80040000-0x80080000) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void class_clear_n(s32 a0);

extern u8 cal_cw_prio;
extern u8 cal_cw_mode;
extern u32 cal_cw_attr;
extern u8 cal_cw_disp;
extern u32 cal_cw_img;
extern u32 cal_cw_anim_tbl;
extern u32 cal_cw_hdr;
extern s16 cal_cw_aux14;
extern u8 cal_cw_flag4;
extern u8 cal_cw_sel;
extern s16 cal_cw_frame;
extern s16 cal_cw_x;
extern s16 cal_cw_y;

extern u32 basyo_init_tbl1;
extern u32 basyo_init_tbl2;
extern u32 _sys_default_tpage_set_tbl2;
extern s16 basyo_init_tbl3;

void data_save_load_class_init(void)
{
    class_clear_n(96);
    cal_cw_prio = 3;
    cal_cw_mode = 64;
    cal_cw_attr = 0x40000000;
    cal_cw_disp = 132;
    cal_cw_img = (u32)&basyo_init_tbl1;
    cal_cw_anim_tbl = (u32)&basyo_init_tbl2;
    cal_cw_hdr = (u32)&_sys_default_tpage_set_tbl2;
    cal_cw_aux14 = basyo_init_tbl3;
    cal_cw_flag4 = 8;
    cal_cw_sel = 1;
    cal_cw_frame = 67;
    cal_cw_x = -16;
    cal_cw_y = -19;
}
