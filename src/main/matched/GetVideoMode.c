/* Address: 0x800B42B4, Size: 16 bytes */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"
extern s32 libapi_video_mode;
s32 GetVideoMode(void) {
    return libapi_video_mode;
}
