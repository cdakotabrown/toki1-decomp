/* Address: 0x800B43C4 */
/* Size: 56 bytes (14 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u16 libapi_vsync_intr_enabled;
extern s32 libapi_vsync_callback;

s32 setIntrVSync(s32 val) {
    s32 old;

    if (libapi_vsync_intr_enabled != 0) {
        old = libapi_vsync_callback;
        if (val != old) {
            libapi_vsync_callback = val;
        }
    }
    return old;
}
