/* Address: 0x8007B6DC */
/* Size: 148 bytes (37 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: swap_instructions --offset1 0x48 --offset2 0x4c */
/* @PostProc: swap_instructions --offset1 0x8c --offset2 0x90 */
#include "types.h"

extern u16 gSDEvt[];

u16 cmd_que_get(void) {
    u32 count;
    u16 first;
    u16 i;
    u16 *p;

    count = gSDEvt[0];
    if (count > 32) {
        count = 32;
    }
    if (count == 0) {
        gSDEvt[0] = (u16)count;
        return 0;
    }
    first = gSDEvt[1];
    if (count < 2) {
        goto done;
    }
    gSDEvt[0] = (u16)count;
    i = 1;
    do {
        p = &gSDEvt[i];
        p[0] = p[1];
        i++;
    } while ((u32)i < (u32)gSDEvt[0]);
    count = gSDEvt[0];
done:
    gSDEvt[0] = (u16)(count - 1);
    return first;
}
