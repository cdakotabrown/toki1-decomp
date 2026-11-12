/* @SymbolStart: 0x80133EA8 */
/* @SymbolSize: 64 */
/* valen_girl9_fade_step - VALEN overlay, Address: 0x80133EA8, Size: 64 bytes (16 insns) */
#include "types.h"

extern u8 now_girl;
extern void sub_sub_step_up(void);
extern void fade_up_main(void);

void valen_girl9_fade_step(void)
{
    u8 mode;

    mode = now_girl;
    if (mode == 9) {
        sub_sub_step_up();
    } else {
        fade_up_main();
    }
}
