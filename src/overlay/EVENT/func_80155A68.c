/* @SymbolStart: 0x80155A60 */
/* @SymbolSize: 156 */
/* @PostProc: scheduler_normalizer */
/* Address: 0x80155A68 */
/* Size: 156 bytes (39 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern u8 D_800B1746;

extern void func_8004A8EC(s32 arg);
extern void func_80033E88(void);
extern void func_8004C250(void);
extern void func_80011EC4(s32 arg);
extern void func_801103A0(void);

extern volatile s16 D_800B0A06;
extern volatile s16 D_800B0A0A;
extern volatile s16 D_800B0A0E;
extern volatile s16 D_80094730;

void func_80155A68(void) {
    u8 step_v = D_800B1746;

    if (step_v == 2) {
        func_8004A8EC(4);
        func_80033E88();
        D_80094730 = 5;
        D_800B0A06 = D_800B0A06 + 1;
        D_800B0A0A = D_800B0A0A + 1;
        D_800B0A0E = D_800B0A0E - 20;
        func_8004C250();
        func_80011EC4(92);
    } else {
        func_801103A0();
    }
}
