/* @SymbolStart: 0x801379BC */
/* @SymbolSize: 148 */
/* Address: 0x801379BC (absorbed cross-prologue head) .. 0x801379C4 */
/* Size: 148 bytes (37 instructions) */
#include "types.h"

extern u16 D_80094718;
extern u8 D_800B1746;

extern s32 func_8004A8EC();
extern void func_80033E88();
extern s32 func_8002336C();
extern void func_800F7074();

void func_801379C4(void)
{
    s32 v;

    v = D_80094718;
    if (v != 5) goto L1;
    func_8004A8EC(1);
    func_80033E88();
    goto END;
L1:
    if (v != 9) goto L2;
    func_8004A8EC(2);
    func_80033E88();
    goto END;
L2:
    if (v != 11) goto END;
    func_8004A8EC(func_8002336C(D_800B1746));
    func_80033E88();
END:
    func_800F7074();
}
