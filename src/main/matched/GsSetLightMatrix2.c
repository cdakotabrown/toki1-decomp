/* Address: 0x8009AF08 */
/* Size: 48 bytes (12 instructions) */
/* Region: Library (>= 0x80086000) */
/* @Compiler: gcc281 */
/* @CompileFlags: --konami-lib-sched */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern s8 GsLIGHTWSMATRIX;
extern void *MulMatrix2(s8 *arg0, s32 arg1);
extern void SetLightMatrix(void *arg0);

void GsSetLightMatrix2(s32 arg0) {
    SetLightMatrix(MulMatrix2(&GsLIGHTWSMATRIX, arg0));
}
