/* Address: 0x8007B67C */
/* Size: 84 bytes (21 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern u16 gSDEvt[];

s32 cmd_que_put(u16 a0) {
    if ((u32)gSDEvt[0] >= 32U) {
        return -1;
    }
    gSDEvt[0]++;
    gSDEvt[gSDEvt[0]] = a0;
    return 0;
}
