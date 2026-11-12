/* Address: 0x801589C0 */
/* Size: 256 bytes */
/* @SymbolSize: 256 */
#include "types.h"

extern u8 *taco_actor_rec;

void taco_type12_retire(void)
{
    s32 i;

    for (i = 16; i < 63; i++) {
        if (taco_actor_rec[i * 136 + 134] == 12) {
            taco_actor_rec[i * 136 + 2] = 254;
        }
    }
}
