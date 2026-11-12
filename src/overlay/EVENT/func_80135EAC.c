/* @SymbolStart: 0x80135EA4 */
/* @SymbolSize: 72 */
/* Address: 0x80135EAC */
/* Size: 72 bytes (18 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern s32 D_80120C20;

extern s32 D_800B1C1C;

extern void func_800137AC(s32, s32, s32, s32, s32);
extern void func_80011DFC(void);

void func_80135EAC(void) {
    s32 a0_v = D_80120C20;

    func_800137AC(320, 0, 64, 128, a0_v);
    D_800B1C1C = 1;
    func_80011DFC();
}
