/* Address: 0x80147678 (cross-prologue head absorbed) .. 0x801476BC */
/* Size: 108 bytes (27 instructions) */
/* @SymbolStart: 0x80147678 */
/* @SymbolSize: 108 */
/* @PostProc: swap_instructions --offset1 0xc --offset2 0x10 */
/* @PostProc: swap_instructions --offset1 0x24 --offset2 0x28 */
/* @PostProc: swap_instructions --offset1 0x20 --offset2 0x24 */
#include "types.h"

extern s16 D_800E9E3E;
extern volatile s16 D_800E9E82;
extern volatile s16 D_800EC194;
extern volatile s16 D_800EC1B8;

extern void func_80011DFC(void);

void func_801476BC(void) {
    D_800E9E82 = D_800E9E82 + 64;
    D_800E9E3E = D_800E9E3E + 64;
    D_800EC194 = D_800EC194 + 64;
    D_800EC1B8 = D_800EC1B8 + 64;
    func_80011DFC();
}
