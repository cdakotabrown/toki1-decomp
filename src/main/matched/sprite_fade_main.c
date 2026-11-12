/* Function: sprite_fade_main */
/* Address: 0x8007EBF0 */
/* Size: 96 bytes (24 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: swap_registers --from v0 --to v1 */
/* @CompileFlags: -Wo,-loopunroll,0 */
#include "types.h"

extern s32 step_counter;

extern void sprite_fade_init(void);
extern void sprite_fade(void);
extern void game_reset(void);

void sprite_fade_main(void) {
    switch (step_counter) {
    case 0:
        sprite_fade_init();
        break;
    case 1:
        sprite_fade();
        break;
    default:
        game_reset();
        break;
    }
}
