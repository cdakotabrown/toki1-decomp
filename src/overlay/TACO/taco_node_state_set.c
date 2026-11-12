/* Address: 0x8015160C */
/* Size: 60 bytes (15 insns) */
#include "types.h"

typedef struct {
    u32 f0;
    s32 f4;
    s32 f8;
    s32 fc;
} Node16;

extern Node16 taco_obj[];

void taco_node_state_set(s32 n, s32 flag)
{
    if (!(flag & 1)) {
        taco_obj[n].f0 = 0x40000000;
    } else {
        taco_obj[n].f0 = 0x80000000;
    }
}
