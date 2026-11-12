/* @SymbolStart: 0x8013364C */
/* @SymbolSize: 108 */
#include "types.h"

extern void memcpy(s32 dst, s32 src, s32 size);
extern void sub_sub_step_up(void);

extern s32 step_counter;

void ending_buf_save(void)
{
    s32 idx = step_counter;
    memcpy(0x801A0000 + idx * 0x2800,
                  0x80180000 + idx * 0x2800, 0x2800);
    step_counter += 1;
    if (step_counter == 9) {
        sub_sub_step_up();
    }
}
