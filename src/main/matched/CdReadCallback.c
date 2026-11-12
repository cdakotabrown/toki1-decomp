/* Address: 0x800879A4, Size: 20 bytes */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: swap_registers v1:v0 --unidirectional */
#include "types.h"
extern int CD_cbread;
s32 CdReadCallback(s32 a) {
    s32 old = CD_cbread;
    CD_cbread = a;
    return old;
}
