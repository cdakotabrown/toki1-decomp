/* Function: magazine_step */
/* Address: 0x80077C6C */
/* Size: 112 bytes (28 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolStart: 0x80077C6C */
/* @SymbolSize: 112 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: swap_registers v0 v1 */
#include "types.h"

extern u8 now_sub_sub_step;

extern void magazine_init(void);
extern void magazine_main(void);
extern void magazine_exit(void);
extern void basyo_show(void);

void magazine_step(void) {
    s32 mode = now_sub_sub_step;

    switch (mode) {
        case 0:
            magazine_init();
            break;
        case 1:
            magazine_main();
            break;
        case 2:
            magazine_exit();
            break;
    }
    basyo_show();
}
