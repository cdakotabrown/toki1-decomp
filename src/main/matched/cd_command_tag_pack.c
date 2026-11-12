/* Address: 0x80046604, Size: 12 bytes (3 instructions) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

void cd_command_tag_pack(u8 a, s32 b, u8 *buf) {
    *(s32 *)buf = b;
    buf[3] = a;
}
