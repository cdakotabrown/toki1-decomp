/* Address: 0x800531F4 */
/* Size: 140 bytes (35 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern s32 p_birthday;
extern s32 girl0_birth[];

s32 birth_day_check_days(s32 a0, s32 a1, s32 a2) {
    s32 slot;
    s32 state;

    slot = a0 & 0xF;

    if (a1 == 13) {
        a1 = 1;
    }

    if (slot == 15) {
        state = p_birthday;
        if (a1 != (state & 0xF)) {
            goto no;
        }
        if (a2 != (s32)((u32)(state << 23) >> 27)) {
            goto no;
        }
        return 1;
    } else {
        state = *(s32 *)((u8 *)girl0_birth + slot * 56);
        if (a1 != (state & 0xF)) {
            goto no;
        }
        if (a2 != (s32)((u32)(state << 23) >> 27)) {
            goto no;
        }
        return 1;
    }
no:
    return 0;
}
