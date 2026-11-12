/* Function: machiawase_main */
/* Address: 0x80076F4C */
/* Size: 136 bytes (34 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolStart: 0x80076F4C */
/* @SymbolSize: 136 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: swap_registers v0 v1 */
#include "types.h"

extern u8 now_sub_sub_step;

extern void machiawase_init(void);
extern void machiawase_select(void);
extern void machiawase_show(void);
extern void machiawase_set(void);
extern void basyo_show(void);

void machiawase_main(void) {
    s32 mode = now_sub_sub_step;

    switch (mode) {
    case 0:
        machiawase_init();
        break;
    case 1:
        machiawase_select();
        break;
    case 2:
        machiawase_show();
        break;
    case 3:
        machiawase_set();
        break;
    }
    basyo_show();
}
