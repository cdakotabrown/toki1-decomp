/* @SymbolStart: 0x8013C4B0 */
/* @SymbolSize: 68 */
/* func_8013C4C8 -- GYOZI overlay, Address: 0x8013C4B0 (prologue 0x8013C4C8), */
/* Size: 68 bytes (17 insns).  D_80148108 += 4 when D_800F53DE >= 97, then */
#include "types.h"

extern void func_8004DE1C(void);

extern u8 D_800F53DE;
extern s16 D_80148108;

void func_8013C4C8(void)
{
    D_80148108 = D_80148108 + (D_800F53DE >= 97U) * 4;
    func_8004DE1C();
}
