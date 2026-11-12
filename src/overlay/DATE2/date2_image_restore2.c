/* @SymbolStart: 0x80132E5C */
/* Address: 0x80132E74, Size: 84 bytes (21 insns) */
/* @SymbolSize: 108 */
#include "types.h"

extern s32 step_counter;

void memcpy(s32 arg0, s32 arg1, s32 arg2);
void sub_sub_step_up(void);

void date2_image_restore2(void)
{
    s32 cnt;
    s32 n;

    n = step_counter * 5120;
    memcpy(n + (s32)0x80180000, n + (s32)0x801C0000, 5120);
    cnt = step_counter;
    cnt++;
    step_counter = cnt;
    if (cnt == 18) {
        sub_sub_step_up();
    }
}
