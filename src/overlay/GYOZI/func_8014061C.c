/* @SymbolSize: 96 */
/* func_8014061C -- GYOZI overlay, Address: 0x8014061C, Size: 96 bytes (24 insns) */
#include "types.h"

extern void func_8004DE1C(void);

extern s8 D_80129F40[];
extern s8 D_80148820;
extern s8 D_80148824;

void func_8014061C(void)
{
    s16 i;

    for (i = 0; i < 6; i++) {
        *(s8 *)(D_80129F40 + i * 68 + 6531) = 0;
    }

    D_80148820 = 0;
    D_80148824 = 0;
    func_8004DE1C();
}
