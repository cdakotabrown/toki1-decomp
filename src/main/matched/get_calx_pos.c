/* Address: 0x8006895C */
/* Size: 88 bytes (22 instructions) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern u8 cal_cell_tbl[];

s16 get_calx_pos(s32 a0, s32 a1) {
    s32 type;
    s32 adjusted;
    s32 remainder;
    s32 row;
    s32 col;
    s32 result;

    type = cal_cell_tbl[a0 * 64];

    if (type == 6) {
        type = type - 7;
    }
    adjusted = type + a1;

    adjusted += 1;
    remainder = adjusted % 7;

    col = a0 * 136;
    row = remainder * 16;

    result = col + row - 120;

    return (s16)result;
}
