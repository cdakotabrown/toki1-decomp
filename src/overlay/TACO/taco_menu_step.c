/* @SymbolStart: 0x80135F7C */
/* @SymbolSize: 184 */
/* Address: 0x80135F8C (window starts at its cross-prologue head, 0x80135F7C) */
#include "types.h"

extern s32 taco_submenu_sel;
extern s32 taco_menu_edit_flag;
extern s32 taco_menu_anim_cnt;

extern void taco_slot0_scale_disp();
extern void taco_slot1_scale_disp();
extern void taco_slot2_scale_disp();
extern void taco_slot3_scale_disp();

void
taco_menu_step(void)
{
    s32 cnt;
    s32 sel;

    cnt = taco_menu_anim_cnt;
    sel = taco_submenu_sel;
    taco_menu_anim_cnt = cnt + 1;
    switch (sel) {
    case 0:
        taco_slot0_scale_disp();
        break;
    case 1:
        taco_slot1_scale_disp();
        break;
    case 2:
        taco_slot2_scale_disp();
        break;
    case 3:
        taco_slot3_scale_disp();
        break;
    }
    if (taco_menu_anim_cnt >= 31) {
        taco_menu_anim_cnt = 0;
        taco_menu_edit_flag = 0;
    }
}
