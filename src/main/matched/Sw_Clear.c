/* Address: 0x80053EB8 */
/* Size: 88 bytes */
/* Region: GCC (0x80041000-0x80080FFF) */
/* Compiler: IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void TestEvent(s32 arg);

extern s32 CE0;
extern s32 CE1;
extern s32 CE2;
extern s32 CE3;

void Sw_Clear(void) {
    TestEvent(CE0);
    TestEvent(CE1);
    TestEvent(CE2);
    TestEvent(CE3);
}
