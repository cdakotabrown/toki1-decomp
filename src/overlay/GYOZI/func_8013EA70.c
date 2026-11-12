/* @SymbolStart: 0x8013EA58 */
/* @SymbolSize: 68 */
/* func_8013EA70 -- GYOZI overlay, Address: 0x8013EA58 (prologue 0x8013EA70), */
/* Size: 68 bytes (17 insns).  D_80148620 += D_800F594E * 3 - 3, then func_8004DE1C(). */
#include "types.h"

extern void func_8004DE1C(void);

extern u8 D_800F594E;
extern s16 D_80148620;

void func_8013EA70(void)
{
    D_80148620 = D_80148620 + D_800F594E * 3 - 3;
    func_8004DE1C();
}
