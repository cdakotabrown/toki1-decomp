/* Address: 0x80087990, Size: 20 bytes */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: swap_registers v1:v0 --unidirectional */
#include "types.h"
extern int CD_cbready;
s32 CdReadyCallback(s32 a) {
    s32 old = CD_cbready;
    CD_cbready = a;
    return old;
}
