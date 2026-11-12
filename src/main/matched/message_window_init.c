/* Address: 0x80065024 */
/* Size: 196 bytes */
/* Compiler: IDO 5.2 */
/* Region: GCC (0x80040000-0x80080000) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void class_clear_n(s32 a0);

extern u8 mes_win_cw_prio;
extern u8 mes_win_cw_mode;
extern u32 mes_win_cw_attr;
extern u8 mes_win_cw_disp;
extern u32 mes_win_cw_img;
extern u32 mes_win_cw_anim_tbl;
extern u32 mes_win_cw_hdr;
extern s16 mes_win_cw_aux14;
extern u8 mes_win_cw_flag4;
extern u8 mes_win_cw_sel;
extern s16 mes_win_cw_frame;
extern s16 mes_win_cw_x;
extern s16 mes_win_cw_y;

extern u32 basyo_init_tbl1;
extern u32 basyo_init_tbl2;
extern u32 _sys_default_tpage_set_tbl2;
extern s16 basyo_init_tbl3;

void message_window_init(void)
{
    class_clear_n(1);
    mes_win_cw_prio = 7;
    mes_win_cw_mode = 64;
    mes_win_cw_attr = 0x40000000;
    mes_win_cw_disp = 132;
    mes_win_cw_img = (u32)&basyo_init_tbl1;
    mes_win_cw_anim_tbl = (u32)&basyo_init_tbl2;
    mes_win_cw_hdr = (u32)&_sys_default_tpage_set_tbl2;
    mes_win_cw_aux14 = basyo_init_tbl3;
    mes_win_cw_flag4 = 8;
    mes_win_cw_sel = 1;
    mes_win_cw_frame = 62;
    mes_win_cw_x = 0;
    mes_win_cw_y = 75;
}
