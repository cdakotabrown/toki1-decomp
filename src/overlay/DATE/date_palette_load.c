/* @SymbolSize: 176 */
/* /* date_palette_load -- DATE overlay, Address: 0x8015463C, Size: 176 bytes (44 insns) */
#include "types.h"

extern void date_bufaddr_init1(void);
extern void palette_upload_enqueue(s32 a, s32 b, s32 c, s32 d, s32 e);
extern void date_sprite_init(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
extern void date_object_set(s32 a, s32 b, s32 c, s32 d);
extern void sub_sub_step_up(void);

extern s32 date_palette_load_res[];
extern u8 date_object_set_tbl0;

void date_palette_load(void)
{
    date_bufaddr_init1();
    palette_upload_enqueue(date_palette_load_res[4], 17, 1, 2, 0);
    date_sprite_init(date_palette_load_res[5], date_palette_load_res[6], date_palette_load_res[7], date_palette_load_res[8], date_palette_load_res[9], date_palette_load_res[10]);
    date_object_set(date_palette_load_res[1], date_palette_load_res[2], date_palette_load_res[0], (*(s16 *)&date_palette_load_res[3]));
    date_object_set_tbl0 = 1;
    sub_sub_step_up();
}
