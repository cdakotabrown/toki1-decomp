/* Address: 0x800451C4 - 0x8004523C (120 bytes) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 seq_busy_check(s32 arg);

s32 device_ready_check(s32 arg0) {
    if (arg0 == 0) {
        if (seq_busy_check(0) != 0 && seq_busy_check(1) != 0) {
            return 1;
        }
        return 0;
    }
    if (seq_busy_check(2) != 0 && seq_busy_check(3) != 0) {
        return 1;
    }
    return 0;
}
