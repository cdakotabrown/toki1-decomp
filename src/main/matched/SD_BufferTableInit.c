/* Address: 0x8007B988 */
/* Size: 164 bytes (41 instructions) */
/* Region: Game */
/* @CompileFlags: -cckr */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u32 gSDHeadAdrs[4];
extern u32 gSDBodyLAdrs[4];
extern u32 gSDBodySize[4];

void SD_BufferTableInit()
{
    gSDHeadAdrs[0] = 0x80010000;
    gSDHeadAdrs[1] = 0x80012000;
    gSDHeadAdrs[2] = 0x80016000;
    gSDHeadAdrs[3] = 0x8001A000;
    gSDBodyLAdrs[0] = 0x1010;
    gSDBodyLAdrs[1] = gSDBodyLAdrs[0] + 0x10000;
    gSDBodyLAdrs[2] = gSDBodyLAdrs[1] + 0x28000;
    gSDBodyLAdrs[3] = gSDBodyLAdrs[2] + 0x28000;
    gSDBodySize[0] = 0x10000;
    gSDBodySize[1] = 0x28000;
    gSDBodySize[2] = 0x28000;
    gSDBodySize[3] = 0x1A768;
}
