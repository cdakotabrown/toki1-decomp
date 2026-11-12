/* Size: 176 bytes */
/* @SymbolSize: 176 */
#include "types.h"
extern s32 taco_submenu_sel;
extern s32 taco_menu_edit_flag;
extern s32 taco_menu_val;
extern s32 pad_trig;
void cursor_step(u8 *p, u32 n)
{
    u32 v;
    n = n & 0xFF;
    if (taco_submenu_sel < 4) { taco_menu_edit_flag = 1; }
    v = *p;
    taco_menu_val = v;
    if ((pad_trig & 0x2020) != 0) { *p = (v + n + 1) % n; }
    if ((pad_trig & 0x8000) != 0) { *p = (*p + n - 1) % n; }
}
