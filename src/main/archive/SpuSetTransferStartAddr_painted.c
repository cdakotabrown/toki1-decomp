/* Function: func_80095628 */
/* Address: 0x80095628 */
/* Size: 108 bytes (27 instructions) */
/* Region: Library (0x80086000+) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: custom_script custom_func_80095628.py */
/* @PostProc: custom_script custom_func_80095628.py */
#include "types.h"

extern u32 _spu_fd;
extern void _spu_ioctl(u32 arg0, s32 arg1, u32 *arg2);

u32 func_80095628(u32 arg0) {
    s32 notAligned;
    if (arg0 >= 0x7FFF9U) {
        return 0;
    }
    notAligned = (arg0 & 7) != 0;
    if (!notAligned) {
        *&arg0 = arg0;
    } else {
        arg0 = (arg0 + 8) & ~7U;
        *&arg0 = arg0;
    }
    _spu_ioctl(_spu_fd, 43, &arg0);
    arg0 = *&arg0;
    return arg0;
}
