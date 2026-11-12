/* Address: 0x8006C7A0 */
/* Size: 104 bytes (26 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: rename_register_chain --chain a2:v1:a0:a1:t0:t1:t2 */
/* @PostProc: rename_register_chain --chain t6:t7:t8:t9:t6 */
#include "types.h"

extern u32 p_birthday;
extern u16 pass_days;
extern u16 get_pass_days_tbl0[];
extern u16 get_pass_days_tbl1;

s32 get_pass_days(void) {
    u32 packed;
    u32 month;
    u32 field;
    s32 diff;
    s32 result;
    u16 v;

    packed = p_birthday;
    month = packed & 0xf;
    field = (packed << 0x17) >> 0x1b;
    diff = (s32)pass_days - (s32)get_pass_days_tbl0[month];
    result = diff - (s32)field + 5475;

    if (month == 4 && field < 4) {
        v = get_pass_days_tbl1;
        result = -(s32)v - (s32)field + 5506;
    }
    return result;
}
