/* @RodataBase: 0x801394B0 */
/* Size: 124 bytes */
/* @SymbolSize: 124 */
#include "types.h"

typedef struct {
    s32 kind;
    s32 rest[11];
} Slot;

extern Slot en_nichi_obj_wk[10];

void en_nichi_obj_count(s32 *pA, s32 *pB)
{
    s32 i;

    *pA = 0;
    *pB = 0;
    for (i = 0; i < 10; i++) {
        switch ((u32)en_nichi_obj_wk[i].kind) {
        case 0:
            break;
        case 1:
        case 2:
        case 3:
            *pA = *pA + 1;
            break;
        case 4:
            *pB = *pB + 1;
            break;
        }
    }
}
