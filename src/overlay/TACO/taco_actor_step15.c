/* @SymbolStart: 0x80158B94 */
/* @SymbolSize: 344 */
/* Address: 0x80158BA4 (real entry 0x80158B94) */
#include "types.h"

extern void taco_actor_exit(void);
extern void taco_effect_spawn(s16, s32, s32);
extern void taco_effect_spawn2(s16, s32, s32);

extern u8 *taco_actor_rec;

#define AC(n) (*(s16 *)(taco_actor_rec + a0 * 136 + (n)))

void taco_actor_step15(s32 a0)
{
    AC(116) = 7000 - AC(122) * 20;
    AC(118) = -1700;
    AC(120) = -20000;

    if (AC(116) < -7000) {
        taco_actor_exit();
    }

    if (AC(116) == -800) {
        taco_effect_spawn(AC(116), AC(118), AC(120));
    }

    if (AC(116) == 0) {
        taco_effect_spawn2(AC(116), AC(118), AC(120));
    }

    if (AC(116) == 800) {
        taco_effect_spawn(AC(116), AC(118), AC(120));
    }
}
