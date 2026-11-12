/* @SymbolStart: 0x80138574 */
/* @SymbolSize: 288 */
/* @RodataBase: 0x8014B64C */
#include "types.h"

extern void name_ent_fuji_hizuke_init(void);
extern void name_ent_fuji_hizuke_input(void);
extern void name_ent_fuji_confirm_init(void);
extern void name_ent_fuji_confirm_input(void);
extern void name_ent_record_step(void);
extern void name_ent_fade_out(void);
extern void name_ent_hizuke_box_disp(s32);
extern void name_ent_hizuke_index_set(s32);
extern void name_ent_confirm_box_disp(s32);
extern void name_ent_confirm_noop(void);

extern s32 step_counter;
extern u8 now_sub_sub_step;

void name_ent_fuji_main(void)
{
    s32 sel;

    step_counter++;
    sel = now_sub_sub_step;
    switch (sel) {
    case 0:
        name_ent_fuji_hizuke_init();
        sel = now_sub_sub_step;
        break;
    case 1:
        name_ent_fuji_hizuke_input();
        sel = now_sub_sub_step;
        break;
    case 2:
        name_ent_fuji_confirm_init();
        sel = now_sub_sub_step;
        break;
    case 3:
        name_ent_fuji_confirm_input();
        sel = now_sub_sub_step;
        break;
    case 4:
        name_ent_record_step();
        sel = now_sub_sub_step;
        break;
    case 5:
        name_ent_fade_out();
        sel = now_sub_sub_step;
        break;
    }
    if ((u32)sel < 4) {
        if ((u32)sel < 2) {
            name_ent_hizuke_box_disp(1);
            name_ent_hizuke_index_set(1);
        } else {
            name_ent_confirm_box_disp(1);
            name_ent_confirm_noop();
        }
    }
}
