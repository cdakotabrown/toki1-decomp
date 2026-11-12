/* Address: 0x8008337C */
/* Size: 96 bytes (24 instructions) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: swap_registers v0:v1 --unidirectional */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 step_counter;

extern void menu2_init(void);
extern void menu_sel(void);
extern void game_reset(void);

void menu2_main(void) {
    switch (step_counter) {
    case 0:
        menu2_init();
        break;
    case 1:
        menu_sel();
        break;
    default:
        game_reset();
        break;
    }
}
