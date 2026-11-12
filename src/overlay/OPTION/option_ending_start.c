/* @SymbolStart: 0x801387F8 */
/* @SymbolSize: 104 */
/* @CompileEnv: IDO52_AS1_PASS1_VOLATILE_OK=2,3 */
/* Address: 0x80138800 (window 0x801387F8..0x80138860, 104 bytes = 26 insns, */
#include "types.h"

extern void class_clear_f(void);
extern void prim_init(void);
extern void goto_step(s32);
extern void goto_sub_step(s32);
extern void goto_sub_sub_step(s32);
extern volatile u8 screen_bri;

s32 option_ending_start(void)
{
    s32 val;

    val = screen_bri;
    if ((u32)val >= 3U) {
        screen_bri = val = (val - 2) & 0xff;
    } else {
        class_clear_f();
        prim_init();
        goto_step(99);
        goto_sub_step(7);
        goto_sub_sub_step(15);
    }
}
