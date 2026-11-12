/* @SymbolStart: 0x8014006C */
/* @SymbolSize: 160 */
/* func_80140074 -- GYOZI overlay, Address: 0x8014006C, Size: 160 bytes (40 insns) */
#include "types.h"

extern void func_8004DE1C(void);

extern u8 D_801486E4;
extern u8 D_8012B94B;
extern u8 D_8012B98F;
extern u8 D_8012B9D3;
extern u8 D_8012BA17;
extern u8 D_80148824;

void func_80140074(void)
{
    u32 mode = D_801486E4;

    if (mode != 0 && mode != 7) {
        D_8012B94B |= 0x80;
    }
    if (mode != 0 && mode != 7) {
        D_8012B98F |= 0x80;
    }
    if (mode != 0 && mode != 7) {
        D_8012B9D3 |= 0x80;
    }
    D_8012BA17 |= 0x80;
    D_80148824 = 1;
    func_8004DE1C();
}
