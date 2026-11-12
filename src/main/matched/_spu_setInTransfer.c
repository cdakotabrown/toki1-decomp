/* Address: 0x800955CC */
/* Size: 40 bytes (10 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @CompileEnv: IDO52_AS1_NO_LA_PULL=all */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern s32 _spu_inTransfer;

void _spu_setInTransfer(s32 arg0) {
    if (arg0 == 1) {
        _spu_inTransfer = 0;
    } else {
        _spu_inTransfer = 1;
    }
}
