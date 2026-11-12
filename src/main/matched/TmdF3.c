/* Function: TmdF3 */
/* Address: 0x8009BEA0 */
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

void GsTMDdivF3NL();
void GsTMDdivF3LFG();
void GsTMDdivF3L();
void GsTMDfastF3NL();
void GsTMDfastF3LFG();
void GsTMDfastF3L();

void TmdF3(s32 a0, s32 a1, s32 a2, s32 a3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    s32 temp_v1;
    s32 pad[2];
    s32 flag = GsNDIV;

    if (flag != 0) {
        if (GsLIOFF == 1) {
            GsTMDdivF3NL(a0, a1, a2, a3, arg4, arg5, arg6, arg7);
            return;
        }
        temp_v1 = GsLIGNR;
        if ((temp_v1 == 0 && GsLIGHT_MODE == 1) || (temp_v1 == 1 && GsLMODE == temp_v1)) {
            GsTMDdivF3LFG(a0, a1, a2, a3, arg4, arg5, arg6, arg7);
            return;
        }
        GsTMDdivF3L(a0, a1, a2, a3, arg4, arg5, arg6, arg7);
        return;
    }
    if (GsLIOFF == 1) {
        GsTMDfastF3NL(a0, a1, a2, a3, arg4, arg5, arg6);
        return;
    }
    temp_v1 = GsLIGNR;
    if ((temp_v1 == 0 && GsLIGHT_MODE == 1) || (temp_v1 == 1 && GsLMODE == temp_v1)) {
        GsTMDfastF3LFG(a0, a1, a2, a3, arg4, arg5, arg6);
        return;
    }
    GsTMDfastF3L(a0, a1, a2, a3, arg4, arg5, arg6);
}
