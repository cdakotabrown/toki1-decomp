/* covers it.  The duplicate carried `@SymbolStart: 0x8006C808`, which */
/* Debug Symbol: album_message_set (from o.bin, high match) */
/* Address: 0x8006C808 */
/* Size: 232 bytes (58 instructions) */
/* Compiler: IDO 5.2 -O2 -G0 -mips1 -EL -non_shared */
/* Region: GCC (0x80041000-0x80080FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "common.h"

extern void class_clear_n(s32 arg0);

extern u8 konami_mark_cw_prio;
extern u8 konami_mark_cw_mode;
extern u8 konami_mark_cw_disp;
extern s32 konami_mark_cw_attr;
extern void *konami_mark_cw_img;
extern void *konami_mark_cw_anim_tbl;
extern void *konami_mark_cw_hdr;
extern s16 konami_mark_cw_aux14;
extern u8 konami_mark_cw_flag4;
extern u8 konami_mark_cw_sel;
extern u8 konami_mark_cw_clut;
extern s16 konami_mark_cw_anim;
extern s16 konami_mark_cw_frame;
extern s16 konami_mark_cw_x;
extern s16 konami_mark_cw_y;

extern u8 basyo_init_tbl1;
extern u8 basyo_init_tbl2;
extern u8 _sys_default_tpage_set_tbl2;
extern s16 basyo_init_tbl3;

void konami_mark_set(s32 arg0)
{
    class_clear_n(15);

    konami_mark_cw_prio = 0;
    konami_mark_cw_mode = 64;
    konami_mark_cw_attr = 0;

    if (arg0 != 0) {
        konami_mark_cw_disp = 140;
    } else {
        konami_mark_cw_disp = 0;
    }

    konami_mark_cw_img = &basyo_init_tbl1;
    konami_mark_cw_anim_tbl = &basyo_init_tbl2;
    konami_mark_cw_hdr = &_sys_default_tpage_set_tbl2;
    konami_mark_cw_aux14 = basyo_init_tbl3;

    konami_mark_cw_flag4 = 8;
    konami_mark_cw_sel = 1;
    konami_mark_cw_clut = 0;
    konami_mark_cw_anim = 0;
    konami_mark_cw_frame = 173;
    konami_mark_cw_x = -134;
    konami_mark_cw_y = -92;
}
