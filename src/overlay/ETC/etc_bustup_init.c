/* @SymbolSize: 108 */
/* Address: 0x8013ED7C */
/* Size: 116 bytes (29 instructions) */
#include "types.h"

extern s32 etc_rec_variant;
extern s32 etc_bustup_res;
extern s32 etc_bustup_res_img;
extern s32 etc_bustup_res_anim_tbl;
extern s32 etc_bustup_res_pal;

extern void sprite_init();
extern void bustup_sprite_init();
extern void bustup_cw_y_layout();
extern void etc_bustup_set();

void etc_bustup_init(void)
{
    etc_bustup_set();
    sprite_init(0, 64);
    bustup_sprite_init(etc_bustup_res_pal, etc_bustup_res, etc_bustup_res_img, etc_bustup_res_anim_tbl, etc_rec_variant);
    bustup_cw_y_layout(etc_bustup_res_img);
}
