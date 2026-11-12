/* @SymbolStart: 0x801561E4 */
/* @SymbolSize: 140 */
/* Address: 0x801561EC */
/* Size: 140 bytes (35 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern u8 D_800B1746;

extern void func_8004A8EC(s32 arg);
extern void func_80033E88(void);
extern void func_8004CF30(void);
extern void func_80011DFC(void);

extern u8 D_8012531C;
extern s32 D_800EECBC;
extern s32 D_800B1AF0;

void func_801561EC(void) {
    u8 step_v = D_800B1746;

    s32 count;
    s32 first;

    if (step_v == 5 && D_8012531C == 0 && D_800EECBC == 2) {
        count = D_800B1AF0;
        first = (u32)count < 1;
        D_800B1AF0 = count + 1;
        if (first) {
            func_8004A8EC(3);
            func_80033E88();
        }
        func_8004CF30();
    } else {
        func_80011DFC();
    }
}
