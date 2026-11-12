/* @SymbolStart: 0x80140D48 */
/* @SymbolSize: 108 */
#include "types.h"

extern u8 D_800B0866[3];
extern s32 func_80011DFC(void);

s32 func_80140D88(void)
{
    D_800B0866[0] += 16;
    D_800B0866[1] += 16;
    D_800B0866[2] += 16;
    if (D_800B0866[0] >= 240U) {
        func_80011DFC();
    }
}
