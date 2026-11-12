/* Address: 0x8007B65C */
/* Size: 32 bytes */
/* Region: GCC (0x80041000-0x80080FFF) */
/* Compiler: IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void SsVoKeyOff(void);

void voice_off(void) {
    SsVoKeyOff();
}
