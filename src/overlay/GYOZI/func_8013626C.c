/* @SymbolStart: 0x80136264 */
/* @SymbolSize: 56 */
/* /* func_8013626C -- GYOZI overlay, Address: 0x8013626C, Size: 48 bytes (12 insns). */
#include "types.h"

extern void func_8013829C(void);
extern void func_8004DE1C(void);
extern s16 D_800D9248;
extern s16 D_800D924C;

void func_8013626C(void)
{
    D_800D9248 = 1;
    D_800D924C = 0;
    func_8013829C();
    func_8004DE1C();
}
