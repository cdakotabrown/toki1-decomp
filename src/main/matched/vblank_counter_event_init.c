/* Address: 0x8004267C */
/* Size: 116 bytes (29 instructions) */
/* Compiler: IDO 5.2 */
/* Region: GCC (0x80041000-0x80080FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "common.h"

extern void EnterCriticalSection(void);
extern s32 OpenEvent(u32 a0, s32 a1, s32 a2, void *callback);
extern void EnableEvent(s32 a0);
extern void SetRCnt(u32 a0, s32 a1, s32 a2);
extern void StartRCnt(u32 a0);
extern void ExitCriticalSection(void);

extern void vblank_counter_tick(void);

extern s32 VB;

void vblank_counter_event_init(void) {
    s32 result;

    EnterCriticalSection();
    result = OpenEvent(0xF2000003, 2, 4096, vblank_counter_tick);
    VB = result;
    EnableEvent(result);
    SetRCnt(0xF2000003, 1, 4096);
    StartRCnt(0xF2000003);
    ExitCriticalSection();
}
