/* Address: 0x800689B4 */
/* Size: 72 bytes (18 instructions) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern u8 cal_cell_tbl[];

s16 get_caly_pos(s32 a0, s32 a1) {
    s32 type;
    s32 adjusted;
    s32 quotient;
    s32 result;

    type = cal_cell_tbl[a0 * 64];

    if (type == 6) {
        type = type - 7;
    }
    adjusted = type + a1;

    adjusted += 1;
    quotient = adjusted / 7;

    result = quotient * 16 - 48;

    return (s16)result;
}
