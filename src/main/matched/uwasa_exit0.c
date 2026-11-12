/* Address: 0x80060E00 - 0x80060E20 (32 bytes) */
/* Region: GCC (OR encoding) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void goto_step(s32 arg0);

void uwasa_exit0(void) {
    goto_step(49);
}
