/* Address: 0x8007E6E4 */
/* Size: 100 bytes (25 instructions) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: swap_registers v0:v1 --unidirectional */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 step_counter;

extern void fade_down_init(void);
extern void fade_down(void);
extern void game_reset(void);
extern void bg_show_girl_out(s32);

void fade_down_main(void) {
    switch (step_counter) {
    case 0:
        fade_down_init();
        break;
    case 1:
        fade_down();
        break;
    default:
        game_reset();
        break;
    }
    bg_show_girl_out(1);
}
