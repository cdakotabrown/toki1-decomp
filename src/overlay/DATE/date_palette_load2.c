/* @SymbolSize: 176 */
/* /* date_palette_load2 -- DATE overlay, Address: 0x8014F734, Size: 176 bytes (44 insns) */
#include "types.h"

extern void date_girl0_mode5_res_ptr_set(void);
extern void palette_upload_enqueue(s32 a, s32 b, s32 c, s32 d, s32 e);
extern void date_sprite_init(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
extern void date_object_set(s32 a, s32 b, s32 c, s32 d);
extern void sub_sub_step_up(void);

extern s32 date_ev04_res;
extern s32 date_ev04_res_cels;
extern s32 date_ev04_res_anim_tbl;
extern s16 date_ev04_res_scale;
extern s32 date_ev04_res_pal;
extern s32 date_ev04_res_bg0;
extern s32 date_ev04_res_bg1;
extern s32 date_ev04_res_bg2;
extern s32 date_ev04_res_bg3;
extern s32 date_ev04_res_bg4;
extern s32 date_ev04_res_bg5;
extern u8 date_object_set_tbl0;

void date_palette_load2(void)
{
    date_girl0_mode5_res_ptr_set();
    palette_upload_enqueue(date_ev04_res_pal, 17, 1, 2, 0);
    date_sprite_init(date_ev04_res_bg0, date_ev04_res_bg1, date_ev04_res_bg2, date_ev04_res_bg3, date_ev04_res_bg4, date_ev04_res_bg5);
    date_object_set(date_ev04_res_cels, date_ev04_res_anim_tbl, date_ev04_res, date_ev04_res_scale);
    date_object_set_tbl0 = 1;
    sub_sub_step_up();
}
