/* Address: 0x80084A54 */
/* Size: 96 bytes (24 instructions) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: swap_registers v0 v1 */
#include "types.h"

extern s32 step_counter;

extern void girl_menu_set(s32 arg0);
extern void girl_menu_select(s32 arg0);
extern void game_reset(void);

void holiday_switch0(void) {
    switch (step_counter) {
    case 0:
        girl_menu_set(0);
        break;
    case 1:
        girl_menu_select(0);
        break;
    default:
        game_reset();
        break;
    }
}
