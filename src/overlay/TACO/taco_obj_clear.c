/* Address: 0x801347B8 */
/* Size: 76 bytes (19 insns) */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: swap_instructions --offset1 0x28 --offset2 0x2c */
#include "types.h"

extern u32 taco_obj[];
extern u32 taco_coord[];

extern void all_object_disp_off();

void taco_obj_clear(void)
{
    u32 *v1;
    u32 *end;

    all_object_disp_off();

    v1 = taco_obj;
    end = taco_coord;

    do {
        v1 += 16;
        v1[-12] = 0x80000000;
        v1[-8] = 0x80000000;
        v1[-4] = 0x80000000;
        v1[-16] = 0x80000000;
    } while (v1 != end);
}
