/* Address: 0x8008B230, Size: 28 bytes */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"
extern int StSTART_FLAG;
extern int StStartFrame;
extern int StEndFrame;
void StSetMask(s32 a, s32 b, s32 c) {
    StSTART_FLAG = a;
    StStartFrame = b;
    StEndFrame = c;
}
