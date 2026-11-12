/* Address: 0x800B42A0 */
/* Size: 20 bytes (5 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 -G0 -mips1 --fill-earliest */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern s32 libapi_video_mode;

s32 SetVideoMode(s32 mode) {
    s32 old = libapi_video_mode;
    libapi_video_mode = mode;
    return old;
}
