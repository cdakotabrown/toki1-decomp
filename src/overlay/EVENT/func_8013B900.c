/* @SymbolStart: 0x8013B8F8 */
/* @SymbolSize: 220 */
/* @PostProc: scheduler_normalizer --const-fp --store-fp --branch-fp */
/* Address: 0x8013B900 */
/* Size: 220 bytes (55 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern s32 D_800EECBC;

extern volatile u16 D_80094714;
extern volatile s16 D_80094718;
extern volatile s16 D_800B0AA2;
extern volatile s16 D_800B0AA6;
extern volatile s16 D_800B0AAA;

extern void func_8004C250(void);
extern void func_80011DFC(void);

void func_8013B900(void) {
    s32 t6_v = D_800EECBC;

    if (t6_v != 1) {
        goto other;
    }
    D_80094714 = (u16)(D_80094714 + 2);
    D_80094718 = 0;
    D_800B0AA2 = (s16)(D_800B0AA2 + 2);
    D_800B0AA6 = (s16)(D_800B0AA6 + 2);
    D_800B0AAA = (s16)(D_800B0AAA - 20);
    func_8004C250();
    func_80011DFC();
    goto shared;
other:
    D_800B0AA2 = (s16)(D_800B0AA2 + 3);
    D_800B0AA6 = (s16)(D_800B0AA6 + 1);
    D_800B0AAA = (s16)(D_800B0AAA - 20);
    func_8004C250();
shared:
    func_80011DFC();
}
