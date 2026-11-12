/* Function: menu3_main */
/* Address: 0x80083178 */
/* Size: 96 bytes (24 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: swap_registers --from v0 --to v1 */
/* @CompileFlags: -Wo,-loopunroll,0 */
#include "types.h"

extern s32 step_counter;

extern void menu3_init(void);
extern void menu3_select(void);
extern void game_reset(void);

void menu3_main(void) {
    switch (step_counter) {
    case 0:
        menu3_init();
        break;
    case 1:
        menu3_select();
        break;
    default:
        game_reset();
        break;
    }
}
