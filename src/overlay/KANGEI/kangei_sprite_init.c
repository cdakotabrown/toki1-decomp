/* @SymbolSize: 140 */
#include "types.h"

extern void palette_upload_enqueue(s32, s32, s32, s32, s32);
extern void date_sprite_init(s32, s32, s32, s32, s32, s32);
extern void date_object_set(s32, s32, s32, s32);
extern void sub_sub_step_up(void);

extern s32 kangei_sprite_buf_tbl[];

void kangei_sprite_init(void)
{
    palette_upload_enqueue(kangei_sprite_buf_tbl[0], 17, 1, 2, 0);
    date_sprite_init(kangei_sprite_buf_tbl[1], kangei_sprite_buf_tbl[2], kangei_sprite_buf_tbl[3], kangei_sprite_buf_tbl[4],
                  kangei_sprite_buf_tbl[5], kangei_sprite_buf_tbl[6]);
    date_object_set(0, 0, 0, 0);
    sub_sub_step_up();
}
