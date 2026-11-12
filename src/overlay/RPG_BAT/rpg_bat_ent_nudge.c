/* Address: 0x801401D0 */
/* Size: 52 bytes */
/* @SymbolSize: 52 */
#include "types.h"

extern u8 class_w[][68];

void rpg_bat_ent_nudge(s32 a0)
{
    u8 *p;

    p = class_w[a0];
    *(s16 *)(p + 6838) = *(s16 *)(p + 6838) + 1;
    *(s16 *)(p + 6842) = *(s16 *)(p + 6842) - 1;
}
