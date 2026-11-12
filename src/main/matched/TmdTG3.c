/* Function: TmdTG3 */
/* Address: 0x8009C60C */
/* Size: 380 bytes (95 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 -G0 -mips1 -fno-delayed-branch -fno-schedule-insns2 --jal-load-nop --konami-lib-sched */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern s32 GsNDIV;
extern s32 GsLIOFF;
extern s32 GsLIGNR;
extern s32 GsLIGHT_MODE;
extern s32 GsLMODE;

void GsTMDdivTG3NL();
void GsTMDdivTG3LFG();
void GsTMDdivTG3L();
void GsTMDfastTG3NL();
void GsTMDfastTG3LFG();
void GsTMDfastTG3L();

void TmdTG3(s32 a0, s32 a1, s32 a2, s32 a3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    s32 temp_v1;
    s32 flag = GsNDIV;

    if (flag != 0) {
        if (GsLIOFF == 1) {
            GsTMDdivTG3NL(a0, a1, a2, a3, arg4, arg5, arg6, arg7);
            return;
        }
        temp_v1 = GsLIGNR;
        if ((temp_v1 == 0 && GsLIGHT_MODE == 1) || (temp_v1 == 1 && GsLMODE == temp_v1)) {
            GsTMDdivTG3LFG(a0, a1, a2, a3, arg4, arg5, arg6, arg7);
            return;
        }
        GsTMDdivTG3L(a0, a1, a2, a3, arg4, arg5, arg6, arg7);
        return;
    }
    if (GsLIOFF == 1) {
        GsTMDfastTG3NL(a0, a1, a2, a3, arg4, arg5, arg6);
        return;
    }
    temp_v1 = GsLIGNR;
    if ((temp_v1 == 0 && GsLIGHT_MODE == 1) || (temp_v1 == 1 && GsLMODE == temp_v1)) {
        GsTMDfastTG3LFG(a0, a1, a2, a3, arg4, arg5, arg6);
        return;
    }
    GsTMDfastTG3L(a0, a1, a2, a3, arg4, arg5, arg6);
}
