/* @SymbolSize: 52  * Status: BYTE-PERFECT 🎉 */
/* func_80143800 -- GYOZI overlay, Address: 0x80143800, Size: 52 bytes (13 insns), Semi-matched */
#include "types.h"

extern void func_8008A0D4(s32 a0);
extern void func_80090960(s32 a0, s32 a1);
extern void func_8004DE1C(void);

void func_80143800(void)
{
    func_8008A0D4(18200);
    func_80090960(4, 4);
    func_8004DE1C();
}
