/* @SymbolSize: 296 */
#include "types.h"

extern void shougatu_bank_set(void);
extern void palette_upload_enqueue(s32, s32, s32, s32, s32);
extern void date_sprite_init(s32, s32, s32, s32, s32, s32);
extern void date_object_set(s32, s32, s32, s32);
extern void sub_sub_step_up(void);

extern u8 now_month;
extern u8 date_object_set_tbl0;
extern s32 shougatu_date_res[];

void shougatu_date_sprite_init(void)
{
    s32 v0;

    shougatu_bank_set();
    v0 = now_month;
    if ((u32)v0 >= 6 && (u32)v0 < 10) {
        palette_upload_enqueue(shougatu_date_res[4], 17, 1, 2, 0);
        date_sprite_init(shougatu_date_res[6], shougatu_date_res[7], shougatu_date_res[8], shougatu_date_res[9],
                      shougatu_date_res[10], shougatu_date_res[11]);
    } else {
        palette_upload_enqueue(shougatu_date_res[5], 17, 1, 2, 0);
        date_sprite_init(shougatu_date_res[12], shougatu_date_res[13], shougatu_date_res[14], shougatu_date_res[15],
                      shougatu_date_res[16], shougatu_date_res[17]);
    }
    date_object_set(shougatu_date_res[1], shougatu_date_res[2], shougatu_date_res[0], (*(s16 *)&shougatu_date_res[3]));
    date_object_set_tbl0 = 1;
    sub_sub_step_up();
}
