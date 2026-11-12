/* Function: TmdTF4 */
/* Address: 0x8009C788 */
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

void GsTMDdivTF4NL();
void GsTMDdivTF4LFG();
void GsTMDdivTF4L();
void func_800A8B40();
void GsTMDfastTF4LFG();
void func_800A8CDC();

void TmdTF4(s32 a0, s32 a1, s32 a2, s32 a3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    s32 temp_v1;
    s32 pad[2];
    s32 flag = GsNDIV;

    if (flag != 0) {
        if (GsLIOFF == 1) {
            GsTMDdivTF4NL(a0, a1, a2, a3, arg4, arg5, arg6, arg7);
            return;
        }
        temp_v1 = GsLIGNR;
        if ((temp_v1 == 0 && GsLIGHT_MODE == 1) || (temp_v1 == 1 && GsLMODE == temp_v1)) {
            GsTMDdivTF4LFG(a0, a1, a2, a3, arg4, arg5, arg6, arg7);
            return;
        }
        GsTMDdivTF4L(a0, a1, a2, a3, arg4, arg5, arg6, arg7);
        return;
    }
    if (GsLIOFF == 1) {
        func_800A8B40(a0, a1, a2, a3, arg4, arg5, arg6);
        return;
    }
    temp_v1 = GsLIGNR;
    if ((temp_v1 == 0 && GsLIGHT_MODE == 1) || (temp_v1 == 1 && GsLMODE == temp_v1)) {
        GsTMDfastTF4LFG(a0, a1, a2, a3, arg4, arg5, arg6);
        return;
    }
    func_800A8CDC(a0, a1, a2, a3, arg4, arg5, arg6);
}
