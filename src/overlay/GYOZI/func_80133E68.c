/* func_80133E68 -- GYOZI overlay, Address: 0x80133E68, Size: 52 bytes (13 insns), Reloc-only */
#include "types.h"

extern void func_80090960(s32 a0, s32 a1);
extern void func_8008A0D4(s32 a0);
extern void func_8004DE1C(void);

void func_80133E68(void)
{
    func_80090960(5, 1);
    func_8008A0D4(16789);
    func_8004DE1C();
}
