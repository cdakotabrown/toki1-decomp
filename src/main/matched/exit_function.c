/* Address: 0x80056670 */
/* Size: 240 bytes (60 instructions) */
/* Compiler: IDO 5.2 -O2 -G0 -mips1 -EL -non_shared */
/* Region: GCC (0x80041000-0x80080FFF) */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count 4 */
#include "common.h"

extern void DrawSync(s32 arg0);
extern void VSync(s32 arg0);
extern void SsEnd(void);
extern void CdControlB(s32 arg0, u8 *arg1, s32 arg2);
extern void DecDCToutCallback(s32 arg0);
extern void CdDataCallback(s32 arg0);
extern void CdReadyCallback(s32 arg0);
extern void CloseEvent(void *arg0);
extern void StopRCnt(u32 arg0);
extern void ResetGraph(s32 arg0);
extern void PadStop(void);
extern void StopCallback(void);
extern void ResetCallback(void);
extern void _96_init(void);
extern void EnterCriticalSection(void);
extern void LoadExec(void *arg0, u32 arg1, s32 arg2);
extern void _96_remove(void);

extern void *VB;
extern u8 exit_function_str0;

void exit_function(void)
{
    s32 unused;
    u8 temp;

    DrawSync(0);
    VSync(0);
    SsEnd();

    temp = 0x80;
    CdControlB(14, &temp, 0);

    DecDCToutCallback(0);
    CdDataCallback(0);
    CdReadyCallback(0);

    CloseEvent(VB);

    StopRCnt(0xF2000000);
    StopRCnt(0xF2000001);
    StopRCnt(0xF2000002);
    StopRCnt(0xF2000003);

    ResetGraph(3);
    PadStop();
    StopCallback();
    ResetCallback();
    _96_init();
    EnterCriticalSection();

    LoadExec(&exit_function_str0, 0x801FFFF0, 0);
    _96_remove();
}
