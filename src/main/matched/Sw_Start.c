/* Address: 0x80053C28 */
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

extern s32 CE0;
extern s32 CE1;
extern s32 CE2;
extern s32 CE3;

void Sw_Start(void)
{
    EnterCriticalSection();

    CE0 = OpenEvent(0xF4000001, 4, 0x2000, 0);

    CE1 = OpenEvent(0xF4000001, 0x8000, 0x2000, 0);

    CE2 = OpenEvent(0xF4000001, 0x100, 0x2000, 0);

    CE3 = OpenEvent(0xF4000001, 0x2000, 0x2000, 0);

    EnableEvent(CE0);
    EnableEvent(CE1);
    EnableEvent(CE2);
    EnableEvent(CE3);

    ExitCriticalSection();
}
