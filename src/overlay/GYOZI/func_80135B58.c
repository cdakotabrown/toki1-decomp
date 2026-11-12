/* @SymbolStart: 0x80135B50 */
/* @SymbolSize: 44 */
/* func_80135B58 -- GYOZI overlay, Address: 0x80135B50, Size: 44 bytes (11 insns) */
#include "types.h"

extern void func_8004DE1C(void);

extern u8 D_80146364;
extern s32 D_8012E66C;

void func_80135B58(void)
{
    s32 temp = D_80146364;

    D_8012E66C = temp;
    func_8004DE1C();
}
