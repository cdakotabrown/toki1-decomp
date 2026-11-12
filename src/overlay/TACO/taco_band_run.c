/* @SymbolSize: 112 */
/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=1 */
/* Address: 0x8014ED9C */
/* Size: 112 bytes (28 insns) */
#include "types.h"

extern u8 *taco_actor_rec;

extern void taco_band_step();
extern void taco_band_angle_init();

void taco_band_run(s32 a0)
{
    u8 *ptr;

    if (a0 == 1) {
        ptr = taco_actor_rec;
        ptr[2990] = 1;
        taco_band_angle_init();
        return;
    }

    if (a0 >= 2 && a0 < 50) {
        ptr = taco_actor_rec;
        if (ptr[2990] == 1) {
            taco_band_step(0);
        }
    }
}
