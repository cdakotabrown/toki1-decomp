/* Likely Symbol: album_message_set (from o.bin, offset +248 bytes from 0x8006C7F8) */
/* Address: 0x8006C8F0 */
/* Size: 236 bytes (59 instructions) */
/* Compiler: IDO 5.2 -O2 -G0 -mips1 -EL -non_shared */
/* Region: GCC (0x80041000-0x80080FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "common.h"

extern void class_clear_n(s32 arg0);

extern u8 yazirushi_cw_prio;
extern u8 yazirushi_cw_mode;
extern u8 yazirushi_cw_disp;
extern u8 yazirushi_cw_flag4;
extern u8 yazirushi_cw_sel;
extern void *yazirushi_cw_img;
extern void *yazirushi_cw_anim_tbl;
extern s16 yazirushi_cw_frame;
extern s16 yazirushi_cw_x;
extern s16 yazirushi_cw_y;
extern void *yazirushi_cw_hdr;
extern s32 yazirushi_cw_attr;

extern u8 basyo_init_tbl1;
extern u8 basyo_init_tbl2;
extern u8 _sys_default_tpage_set_tbl2;

void yazirushi_disp_switch(s32 arg0)
{
    class_clear_n(2);

    yazirushi_cw_prio = 2;
    yazirushi_cw_mode = 64;
    yazirushi_cw_attr = 0x40000000;

    yazirushi_cw_img = &basyo_init_tbl1;
    yazirushi_cw_anim_tbl = &basyo_init_tbl2;
    yazirushi_cw_hdr = &_sys_default_tpage_set_tbl2;

    yazirushi_cw_frame = 1;
    yazirushi_cw_flag4 = 8;
    yazirushi_cw_sel = 1;
    yazirushi_cw_x = 116;
    yazirushi_cw_y = 72;

    if (arg0 == 1) {
        yazirushi_cw_disp = yazirushi_cw_disp | 0x80;
    } else {
        yazirushi_cw_disp = yazirushi_cw_disp & 0x7F;
    }
}
