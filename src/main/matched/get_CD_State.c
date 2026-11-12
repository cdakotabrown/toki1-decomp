/* Address: 0x80046378, Size: 16 bytes (4 instructions) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern u8 cd_state;

u32 get_CD_State(void) {
    return cd_state;
}
