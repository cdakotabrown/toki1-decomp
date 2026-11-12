/* @SymbolStart: 0x80135450 */
/* @SymbolSize: 252 */
/* @RodataBase: 0x8013CBAC */
/* Address: 0x80135458, Size: 244 bytes (61 instructions) */
#include "types.h"

extern void option_mouse_init();
extern void func_80134804();
extern void option_zoom();
extern void option_submenu_init();
extern void option_mouse_mode();
extern void option_menu3_init();
extern void option_menu2_init();
extern void option_click_mode();
extern void option_menu3_init2();
extern void option_menu2_init2();
extern void option_click_speed();
extern void option_mouse_menu_reset();
extern u8 now_sub_sub_step;

void option_mouse_step(void)
{
    s32 hidden_t6;
    u32 idx;

    hidden_t6 = now_sub_sub_step;
    idx = (u32)hidden_t6;

    switch (idx) {
    case 0:
        option_mouse_init();
        break;
    case 1:
        func_80134804();
        break;
    case 2:
        option_zoom();
        break;
    case 3:
        option_submenu_init();
        break;
    case 4:
        option_mouse_mode();
        break;
    case 5:
        option_menu3_init();
        break;
    case 6:
        option_menu2_init();
        break;
    case 7:
        option_click_mode();
        break;
    case 8:
        option_menu3_init2();
        break;
    case 9:
        option_menu2_init2();
        break;
    case 10:
        option_click_speed();
        break;
    case 11:
        option_mouse_menu_reset();
        break;
    }
}
