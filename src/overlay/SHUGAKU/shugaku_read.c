/* shugaku_read - SHUGAKU overlay, Address: 0x80138368, Size: 56 bytes (14 insns) */
#include "types.h"

extern s32 step_counter;

extern void cd_read_start2(s32 a0, s32 a1, s32 a2);

void
shugaku_read(void)
{
    s32 t6, t7;

    cd_read_start2(13, 0x801D0000, 0xBBA6);
    t6 = step_counter;
    t7 = t6 + 1;
    step_counter = t7;
}
