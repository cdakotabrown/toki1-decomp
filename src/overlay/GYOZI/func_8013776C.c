/* @SymbolStart: 0x80137764 */
/* @SymbolSize: 88 */
/* func_8013776C -- GYOZI overlay, Address: 0x80137764, Size: 88 bytes (22 insns) */
#include "types.h"

extern void func_8004DE1C(void);

extern u8 D_80147240;
extern u8 D_800F62CF;
extern u8 D_800F647A;

void func_8013776C(void)
{
    u32 temp = D_80147240;

    if (temp != 1) goto label0;
    if (D_800F62CF != 9) goto label0;
    D_800F647A = D_800F647A + 6;
    goto label1;

label0:
    func_8004DE1C();

label1:
    ;
}
