/* @SymbolStart: 0x80132ABC */
/* @SymbolSize: 120 */
/* option_exit_fade - OPTION overlay, Address: 0x80132AC4, Size: 112 bytes (28 insns) */
#include "types.h"

extern volatile u8 screen_bri;
extern void text_sys_init(void);
extern void class_clear_all(s32);
extern void goto_step(s32);

void option_exit_fade(void)
{
    s32 val;

    val = screen_bri;
    if ((u32)val >= 2U) {
        val -= 2;
        val &= 0xff;
    }
    screen_bri = val;
    if ((u32)val < 65U) {
        text_sys_init();
    }
    val = screen_bri;
    if ((u32)val < 2U) {
        screen_bri = 0;
        class_clear_all(0);
        goto_step(18);
    }
}
