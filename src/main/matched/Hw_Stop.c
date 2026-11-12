/* Address: 0x80054034 - 0x8005409C (104 bytes) */
/* Region: GCC (< 0x80081000) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void EnterCriticalSection(void);
extern void CloseEvent(s32 arg0);
extern void ExitCriticalSection(void);

extern s32 CEH0;
extern s32 CEH1;
extern s32 CEH2;
extern s32 CEH3;

void Hw_Stop(void) {
    EnterCriticalSection();
    CloseEvent(CEH0);
    CloseEvent(CEH1);
    CloseEvent(CEH2);
    CloseEvent(CEH3);
    ExitCriticalSection();
}
