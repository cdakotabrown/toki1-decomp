/* Function: cd_read */
/* Address: 0x80089890 */
/* Size: 112 bytes (28 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 -G0 -mips1 */
/* @PostProc: strip_leading_instructions --count 19 */
/* @PostProc: custom_script cd_read.py */
#include "types.h"

extern s32 CdIntToPos(s32 a0, s32 *a1);
extern s32 CdControl(s32 a0, s32 *a1, s32 a2);
extern s32 CdRead(s32 a0, s32 a1, s32 a2);
extern s32 CdReadSync(s32 a0, s32 a1);

s32 cd_read(s32 a0, s32 a1, s32 a2) {
    s32 buf[1];
    CdIntToPos(a1, buf);
    CdControl(2, buf, 0);
    CdRead(a0, a2, 128);
    return CdReadSync(0, 0) == 0;
}
