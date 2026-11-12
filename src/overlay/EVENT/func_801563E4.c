/* @SymbolStart: 0x801563DC */
/* @SymbolSize: 100 */
/* Address: 0x801563E4 */
/* Size: 100 bytes (25 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern u8 D_800B1746;

extern s32 D_800EECBC;
extern u8 D_8012531C;

extern void func_8004CF30(void);
extern void func_80011DFC(void);

void func_801563E4(void) {
    u8 t6_v = D_800B1746;

    if (t6_v != 10) goto fallback;
    if (D_800EECBC != 2) goto fallback;
    if (D_8012531C != 4) goto fallback;
    func_8004CF30();
    return;
fallback:
    func_80011DFC();
}
