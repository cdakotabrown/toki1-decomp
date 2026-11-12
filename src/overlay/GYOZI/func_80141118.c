/* @SymbolStart: 0x80141110 */
/* @SymbolSize: 80 */
/* func_80141118 -- GYOZI overlay, Address: 0x80141118, Size: 68 bytes (17 insns) */
#include "types.h"

typedef struct {
    s32 word;
} Rec4;

extern void func_8004DE1C(void);

extern u8 D_800F62CF;
extern Rec4 D_80148970;
extern u8 D_800F53A0[];

void func_80141118(void)
{
    u32 t6 = D_800F62CF;

    *(Rec4 *)(D_800F53A0 + t6 * 56 + 432) = D_80148970;
    func_8004DE1C();
}
