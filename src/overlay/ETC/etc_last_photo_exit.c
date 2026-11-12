/* @SymbolSize: 136 */
/* Address: 0x80146180 */
/* Size: 136 bytes (34 instructions) */
#include "types.h"

extern void screen_clear(void);
extern void prim_init(void);
extern void goto_sub_sub_step(s32);
extern void class_clear_f(void);
extern void SetDispMask(s32);
extern u8 option_on;
extern u32 pad_prev;

void etc_last_photo_exit(void)
{
    prim_init();
    class_clear_f();
    SetDispMask(0);
    screen_clear();

    if (option_on != 0) {
        goto_sub_sub_step(3);
        return;
    }

    if ((pad_prev & 0x0860) != 0) {
        goto_sub_sub_step(1);
        return;
    }

    goto_sub_sub_step(2);
}
