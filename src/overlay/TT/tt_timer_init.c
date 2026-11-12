/* @SymbolStart: 0x8013E290 */
/* @SymbolSize: 124 */
/* Address: 0x8013E290 (cross-prologue head absorbed), Size: 124 bytes */
#include "types.h"

extern void tt_timer();
extern s32 *tt_disp_wk;

extern s32 EnterCriticalSection(void);
extern s32 ExitCriticalSection(void);
extern s32 OpenEvent(u32 id, s32 mode, s32 size, u8 *callback);
extern s32 EnableEvent(s32 handle);
extern void SetRCnt(u32 id, s32 mode, s32 size);
extern void StartRCnt(u32 id);

void tt_timer_init(void)
{
    s32 *result_ptr = tt_disp_wk;
    s32 v0;

    EnterCriticalSection();
    v0 = OpenEvent(0xF2000003, 2, 4096, tt_timer);
    *result_ptr = v0;
    EnableEvent(v0);
    SetRCnt(0xF2000003, 1, 4096);
    StartRCnt(0xF2000003);
    ExitCriticalSection();
}
