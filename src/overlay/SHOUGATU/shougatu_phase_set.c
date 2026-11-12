/* @SymbolSize: 232 */
/* Address: 0x80140768 */
/* Size: 232 bytes (58 instructions). NOTE: the function map over-sizes this */
#include "types.h"

extern u8 class_w[];
extern s32 rand(void);

void shougatu_phase_set(void)
{
    s16 i;
    u8 *p;

    for (i = 0; i < 3; i++) {
        p = class_w + i * 68;
        if (*(s16 *)(p + 6688) == 14) {
            if (*(s16 *)(p + 6672) == 9) {
                *(s16 *)(p + 6686) = rand() % 3;
            }
        }
    }
}
