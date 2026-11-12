/* @SymbolStart: 0x8013C534 */
/* @SymbolSize: 60 */
/* func_8013C53C -- GYOZI overlay, Address: 0x8013C534, Size: 60 bytes (15 insns) */
#include "types.h"

extern void func_8004DE1C(void);
extern void func_8013E148(void);

extern u8 D_80148110;

void func_8013C53C(void)
{
    u32 temp = D_80148110;

    if (temp == 0) goto label0;
    func_8004DE1C();
    goto label1;

label0:
    func_8013E148();

label1:
    ;
}
