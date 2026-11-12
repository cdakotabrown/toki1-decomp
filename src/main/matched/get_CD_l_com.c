/* Address: 0x80046320, Size: 56 bytes */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"
extern u8 cdq_head;
extern u8 cdq_que[];
s32 get_CD_l_com(void) {
    u32 idx = ((u32)cdq_head + 0x2F) % 0x30;
    return cdq_que[idx * 24];
}
