/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_CONST_INREG_VALUES=1 */
/* @PostProc: swap_branch_operands --offsets 0xd0 */
/* @SymbolStart: 0x801556B8 */
/* @SymbolSize: 364 */
/* Address: 0x801556B8 (cross-prologue head; .sym draws the start at 0x801556C0) */
/* Size: 364 bytes (91 instructions) */
#include "types.h"

extern u8 D_800B1746;
extern u8 D_80125320;
extern u8 D_800B0E31;
extern u8 D_800B0896;
extern u8 D_800B0BCE;
extern s16 D_80094730;
extern s16 D_8012438C[];

extern void func_80114B0C(s32);
extern void func_8004A8EC(s32);
extern void func_80033E88(void);
extern s32 func_80011DFC(void);

s32 func_801556C0(void) {
    s32 step;
    s32 mood;
    u8 slot;

    step = D_800B1746;

    if (step == 1 && D_80125320 == 3) {
        mood = D_800B0E31;
        if (mood == 0) {
            slot = D_800B0896;
            if (slot < 3U || slot >= 6U) {
                D_80094730 = D_80094730 + 1U;
                D_8012438C[8] = D_8012438C[9] = D_8012438C[10] = 3;
                D_8012438C[0] = D_8012438C[1] = D_8012438C[2] = 3;
                func_80114B0C(1);
                func_8004A8EC(3);
                func_80033E88();
                return 0;
            }
        }
        if (mood == 1) {
            D_8012438C[8] = D_8012438C[9] = D_8012438C[10] = 4;
            D_8012438C[0] = D_8012438C[1] = D_8012438C[2] = 4;
            func_80114B0C(1);
            func_8004A8EC(4);
            func_80033E88();
            return 0;
        }
    } else if (step == 8 && D_80125320 == 2 && D_800B0BCE != 0) {
        D_80094730 = D_80094730 + 3;
    }

    return func_80011DFC();
}
