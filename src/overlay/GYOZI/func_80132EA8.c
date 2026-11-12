/* @SymbolStart: 0x80132EA0 */
/* @SymbolSize: 60 */
/* func_80132EA8 -- GYOZI overlay, Address: 0x80132EA0, Size: 60 bytes (15 insns) */
#include "types.h"

extern void func_8004DEAC(s32);
extern void func_8004DE1C(void);

extern s32 D_8012E66C;

void func_80132EA8(void)
{
    s32 temp = D_8012E66C;

    if (temp != 0) goto label0;
    func_8004DEAC(3);
    goto label1;

label0:
    func_8004DE1C();

label1:
    ;
}
