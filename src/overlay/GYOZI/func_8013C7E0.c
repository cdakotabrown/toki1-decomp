/* @SymbolStart: 0x8013C7D8 */
/* @SymbolSize: 76 */
/* func_8013C7E0 -- GYOZI overlay, Address: 0x8013C7D8, Size: 76 bytes (19 insns) */
#include "types.h"

extern void func_80090960(s32 a0, s32 a1);
extern void func_8008F618(s32 a0);
extern void func_8004DE1C(void);

extern s32 D_8012E66C;
extern u8 D_800F62CF;

void func_8013C7E0(void)
{
    s32 temp = D_8012E66C;

    D_800F62CF = temp;
    func_80090960(7, 0);
    func_8008F618(D_800F62CF);
    func_8004DE1C();
}
