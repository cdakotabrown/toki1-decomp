/* @SymbolStart: 0x801359C4 */
/* @SymbolSize: 80 */
/* Address: 0x801359CC */
/* Size: 72 bytes (18 insns) */
#include "types.h"

extern s32 taco_menu_edit_flag;

extern void dec_bg_show(s32 arg);
extern void taco_menu_disp(void);
extern void taco_menu_step(void);
extern void taco_submenu(void);

void taco_menu_show(void)
{
    s32 a0 = taco_menu_edit_flag;

    if (a0 == 1) {
        taco_menu_step();
    } else {
        taco_submenu();
    }
    dec_bg_show(2);
    taco_menu_disp();
}
