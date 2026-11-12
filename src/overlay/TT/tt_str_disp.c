#include "types.h"

extern void func_801487C0(s16 x, s16 y, u8 ch);

void tt_str_disp(s16 x, s16 y, u8 *str)
{
    while (*str != 0) {
        func_801487C0(x, y, *str++);
        x += 6;
    }
}
