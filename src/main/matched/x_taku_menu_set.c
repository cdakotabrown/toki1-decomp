/* Function: x_taku_menu_set */
/* Address: 0x80051044 */
/* Size: 340 bytes (85 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern void menu_set(s32 arg0, s32 arg1, s16 *arg2, void *arg3, s16 *arg4, s16 *arg5);
extern void *x_taku_menu_set_str0;

void x_taku_menu_set(s32 arg0, s32 arg1)
{
    s32 pad0[2];
    s16 sp60[4];
    s16 sp52[4];
    s16 sp44[4];
    s32 pad1;
    s32 pad2;
    s32 i;

    (void)&pad0;
    (void)&pad1;
    (void)&pad2;
    for (i = 0; i < arg1; i++) {
        sp60[i] = -120;
        sp52[i] = 240;
        sp44[i] = 16;
    }
    if (arg1 == 3) {
        menu_set(arg0, 3, sp60, &x_taku_menu_set_str0, sp52, sp44);
    } else {
        menu_set(arg0, 2, sp60, &x_taku_menu_set_str0, sp52, sp44);
    }
}
