/* Address: 0x80053904 */
/* Size: 56 bytes (14 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: swap_registers t6:v1 --unidirectional */
#include "types.h"

extern u16 basyo_pack;

s32 basyo_range_check(void) {
    u32 top;

    top = (u32)basyo_pack >> 12;
    if (top >= 6 && top < 11) {
        return 1;
    }
    return 0;
}
