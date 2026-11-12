/* @SymbolStart: 0x80132E80 */
/* @SymbolSize: 340 */
/* @RodataBase: 0x8014B5E8 */
/* Address: 0x80132E88 */
#include "types.h"

extern u8 now_sub_sub_step;

extern void name_ent_panel_disp(s32);
extern void name_ent_cursor_pos_set(void);
extern void name_ent_hizuke_box_disp(s32);
extern void name_ent_hizuke_index_set(s32);
extern void name_ent_confirm_box_disp(s32);
extern void name_ent_confirm_noop(void);

extern void name_ent_init(void);
extern void name_ent_sei_input(void);
extern void name_ent_mei_init(void);
extern void name_ent_mei_input(void);
extern void name_ent_adana_init(void);
extern void name_ent_adana_input(void);
extern void name_ent_hizuke_init(void);
extern void name_ent_hizuke_input(void);
extern void name_ent_confirm_init(void);
extern void name_ent_confirm_input(void);

void name_ent_entry_step(void)
{
    if (now_sub_sub_step != 0) {
        if ((u32)now_sub_sub_step < 6) {
            name_ent_panel_disp(0);
            name_ent_cursor_pos_set();
            now_sub_sub_step = now_sub_sub_step;
        } else if ((u32)now_sub_sub_step < 8) {
            name_ent_hizuke_box_disp(0);
            name_ent_hizuke_index_set(0);
            now_sub_sub_step = now_sub_sub_step;
        } else {
            name_ent_confirm_box_disp(0);
            name_ent_confirm_noop();
            now_sub_sub_step = now_sub_sub_step;
        }
    }

    switch ((u32)now_sub_sub_step) {
    case 0:
        name_ent_init();
        break;
    case 1:
        name_ent_sei_input();
        break;
    case 2:
        name_ent_mei_init();
        break;
    case 3:
        name_ent_mei_input();
        break;
    case 4:
        name_ent_adana_init();
        break;
    case 5:
        name_ent_adana_input();
        break;
    case 6:
        name_ent_hizuke_init();
        break;
    case 7:
        name_ent_hizuke_input();
        break;
    case 8:
        name_ent_confirm_init();
        break;
    case 9:
        name_ent_confirm_input();
        break;
    default:
        break;
    }
}
