/* @SymbolSize: 40 */
/* func_8013E21C -- GYOZI overlay, Address: 0x8013E21C, Size: 40 bytes (10 insns) */
#include "types.h"

extern void func_8004DE1C(void);

extern s32 D_8012E68C;

void func_8013E21C(void)
{
    D_8012E68C = 30;
    func_8004DE1C();
}
