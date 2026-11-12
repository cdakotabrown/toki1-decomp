/* Address: 0x8008797C, Size: 20 bytes */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: swap_registers v1:v0 --unidirectional */
#include "types.h"
extern int CD_cbsync;
s32 CdSyncCallback(s32 a) {
    s32 old = CD_cbsync;
    CD_cbsync = a;
    return old;
}
