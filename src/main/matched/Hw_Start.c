/* Address: 0x80053D0C */
/* Size: 228 bytes (57 instructions) */
/* Region: GCC (0x80041000 - 0x80080FFF) */
/* Compiler: IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void EnterCriticalSection(void);
extern s32 OpenEvent(u32 mode, s32 ch, s32 size, s32 arg3);
extern void EnableEvent(s32 handle);
extern void ExitCriticalSection(void);

extern s32 CEH0;
extern s32 CEH1;
extern s32 CEH2;
extern s32 CEH3;

void Hw_Start(void)
{
    EnterCriticalSection();

    CEH0 = OpenEvent(0xF0000011, 4, 0x2000, 0);

    CEH1 = OpenEvent(0xF0000011, 0x8000, 0x2000, 0);

    CEH2 = OpenEvent(0xF0000011, 0x100, 0x2000, 0);

    CEH3 = OpenEvent(0xF0000011, 0x2000, 0x2000, 0);

    EnableEvent(CEH0);
    EnableEvent(CEH1);
    EnableEvent(CEH2);
    EnableEvent(CEH3);

    ExitCriticalSection();
}
