/* Address: 0x8013EDF0 */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern void bustup_kuchi_ani();
extern s32 etc_bustup_flag;

void etc_bustup_kuchi_ani(void)
{
    s32 t6 = etc_bustup_flag;

    if (t6 != 1) {
        return;
    }
    bustup_kuchi_ani();
}
