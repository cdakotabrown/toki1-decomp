/* Address: 0x800630C4 */
/* Size: 108 bytes (27 instructions) */
/* Compiler: GCC-like (0x80040000-0x80080000 region) */
#include "types.h"

extern void white_light(s32);
extern void goto_step(s32);
extern void game_reset(void);

extern s32 work_fuku;

void func_800630C4(void) {
    s32 val;

    white_light(255);
    val = work_fuku;

    switch (val) {
    case 0:
        goto_step(19);
        break;
    case 1:
        goto_step(32);
        break;
    default:
        game_reset();
        break;
    }
}
