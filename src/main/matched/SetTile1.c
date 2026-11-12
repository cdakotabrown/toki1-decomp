/* Address: 0x800A2EC8, Size: 40 bytes (10 instructions) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: rename_register_chain --chain v0:t6:t7:t8:t9 */
#include "types.h"

void SetTile1(u8 *p) {
    u32 tag = *(u32 *)p;
    tag = (tag & 0x00FFFFFF) | (2u << 24);
    p[7] = 0x68;
    *(u32 *)p = tag;
}
