/* Address: 0x80143A14 */
/* Size: 36 bytes (9 insns) */
#include "types.h"

extern void object3d_init(s32);

void taco_object_slot_init(s32 a0)
{
    object3d_init(a0 * 2 + 2);
}
