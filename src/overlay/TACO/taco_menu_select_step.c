/* @SymbolStart: 0x80135648 */
/* @RodataBase: 0x8015D698 */
/* @SymbolSize: 220 */
/* Address: 0x80135650 */
#include "types.h"

extern s32 step_counter;
extern u8 now_sub_step;
extern u8 taco_menu_csr;

extern void goto_sub_step(s32);
extern void class_switch(s32);
extern void taco_menu_str_disp(void);

void taco_menu_select_step(void)
{
    s32 t6_in = step_counter;

    u8 sel;

    if ((u32)t6_in >= 65U) {
        sel = taco_menu_csr;
        switch (sel) {
            case 0: goto_sub_step(10); break;
            case 1: goto_sub_step(4); break;
            case 2: goto_sub_step(5); break;
            case 3: goto_sub_step(7); break;
            case 4: goto_sub_step(46); break;
        }
    }
    if (now_sub_step != 3) {
        class_switch(0);
    }
    if ((step_counter & 4) != 0) {
        taco_menu_str_disp();
    }
}
