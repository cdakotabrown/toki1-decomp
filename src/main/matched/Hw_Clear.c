/* Address: 0x80053FDC */
/* Size: 88 bytes */
/* Region: GCC (0x80041000-0x80080FFF) */
/* Compiler: IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void TestEvent(s32 arg);

extern s32 CEH0;
extern s32 CEH1;
extern s32 CEH2;
extern s32 CEH3;

void Hw_Clear(void) {
    TestEvent(CEH0);
    TestEvent(CEH1);
    TestEvent(CEH2);
    TestEvent(CEH3);
}
