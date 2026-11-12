/* Likely Symbol: icon_slot_clear_step (from o.bin, offset +44 bytes from 0x80085CC8) */
/* Address: 0x80085CF4 */
/* Size: 96 bytes */
/* Region: GCC (0x80085CF4) */
/* Compiler: IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void class_clear_n(s32 a0);
extern void sub_sub_step_up(void);
extern s32 screen_w[];

void icon_slot_clear_step(void)
{
    s16 i;

    class_clear_n(31);
    class_clear_n(30);

    for (i = 5; i < 10; i++)
    {
        screen_w[i + 1162] = -1;
    }

    sub_sub_step_up();
}
