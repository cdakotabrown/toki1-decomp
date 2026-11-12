/* Address: 0x801439D4 */
#include "types.h"

extern void object3d_init(s32);

void
taco_objects_init(void)
{
    s32 s0;
    s32 s1;

    s0 = 2;
    s1 = 160;
lbl_801439EC:
    object3d_init(s0);
    s0 += 2;
    if (s0 != s1) goto lbl_801439EC;
}
