/* @SymbolSize: 232 */
/* func_80140C14 -- GYOZI overlay, Address: 0x80140C14, Size: 232 bytes (58 insns) */
#include "types.h"

extern s32 func_800BCE40(void);

extern s8 D_80129F40[];

void func_80140C14(void)
{
    s16 i;
    s8 *ptr;

    for (i = 0; i < 3; i++) {
        ptr = D_80129F40 + i * 68;

        if (*(s16 *)(ptr + 6688) != 14) {
            continue;
        }
        if (*(s16 *)(ptr + 6672) != 9) {
            continue;
        }

        *(s16 *)(ptr + 6686) = (s16)(func_800BCE40() % 3);
    }
}
