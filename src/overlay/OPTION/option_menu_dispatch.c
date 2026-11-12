/* @CompileEnv: IDO52_PRECOLOR_WEB=v1:4 */
/* @SymbolStart: 0x8013C7A0 */
/* @SymbolSize: 256 */
#include "types.h"

extern void option_menu0();
extern void option_menu1();
extern void option_menu3();
extern void option_menu4();
extern void option_menuf();
extern void sub_step_up();
extern void goto_sub_step();
extern void update_anim_state();

extern u8 now_sub_step;
extern u32 pad_trig;

void option_menu_dispatch(void) {
    s32 arg0;
    s32 val;

    arg0 = now_sub_step;

    switch (arg0) {
        case 0:
            option_menu0();
            goto after;
        case 1:
            option_menu1();
            goto after;
        case 2:
            option_menu3();
            goto after;
        case 3:
            option_menu4();
            goto after;
        case 255:
        default:
            option_menuf();
            goto after;
    }
after:
    val = now_sub_step;
    if (val != 255) {
        if (pad_trig & 0x20) {
            sub_step_up();
        }
    }
    val = now_sub_step;
    if (val != 0) {
        if (pad_trig & 0x40) {
            goto_sub_step((u8)(val - 1));
        }
    }
    update_anim_state(1);
}
