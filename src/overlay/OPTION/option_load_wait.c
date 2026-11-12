/* @SymbolSize: 60 */
/* Address: 0x801387BC */
/* Size: 60 bytes (15 instructions) */
/* Overlay: OPTION.EXN */
#include "types.h"

extern s32 get_CD_State2();
extern void goto_step();
extern void goto_sub_step();

void option_load_wait(void) {
    if (get_CD_State2() & 1) {
        goto_step(99);
        goto_sub_step(3);
    }
}
