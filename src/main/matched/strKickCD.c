/* Likely Symbol: strKickCD (from o.bin, offset +56 bytes from 0x80056EE4) */
/* Address: 0x80056EAC */
/* Size: 108 bytes (27 instructions) */
/* Compiler: IDO 5.2 */
/* Region: GCC (0x80041000-0x80080FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 CdControl(s32 cmd, s32 param, s32 flag);
extern s32 CdRead2(s32 timeout);

void strKickCD(s32 arg0)
{

    if (CdControl(21, arg0, 0) == 0) {
        do {

        } while (CdControl(21, arg0, 0) == 0);
    }

    if (CdRead2(448) == 0) {
        do {

        } while (CdRead2(448) == 0);
    }
}
