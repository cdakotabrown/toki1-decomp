/* @SymbolSize: 208 */
/* /* @PostProc: swap_instructions --offset1 0x28 --offset2 0x2c */
/* taco_star_init -- TACO overlay, Address: 0x80144C70 */
#include "types.h"

extern s32 rand(void);

void taco_star_init(s32 *out)
{
    s32 i;

    i = 0;
    do {
        out[0] = rand() % 320;
        out[1] = (rand() % 240) - 120;
        i++;
        out += 2;
    } while (i != 25);
}
