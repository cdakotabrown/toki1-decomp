/* @PostProc: patch_stack_complete --old 32 --new 96 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* @PostProc: remap_stack_slots --from 24 --to 88 */
/* @PostProc: scheduler_normalizer --block-aware --store-fp --load-fp */
/* @PostProc: swap_instructions --offset1 0x84 --offset2 0x90 */
/* @PostProc: swap_instructions --offset1 0xc8 --offset2 0xcc */
/* @PostProc: patch_insn_reg_field 0x68:rt:v0:t9 0x6c:rt:a1:t0 0x70:rt:a2:t1 0x84:rt:v0:t9 0x88:rt:a1:t0 0x90:rt:a2:t1 0x98:rt:t9:t2 0x9c:rt:t0:t3 0xa0:rd:t1:t4 0xa0:rs:t0:t3 0xa0:rt:t9:t2 0xa8:rt:t1:t4 0xb4:rt:t6:t9 0xb8:rt:t2:t5 0xbc:rt:t3:t6 0xc0:rd:t4:t7 0xc0:rs:t3:t6 0xc0:rt:t2:t5 0xc4:rt:t5:t8 0xc8:rt:t4:t7 0xcc:rt:t5:t8 0xd4:rt:t6:t9 */
/* Address: 0x8013E340 */
#include "types.h"

typedef struct {
    s16 x;
    s16 y;
    s16 z;
    s16 pad06;
    s16 f08;
    s16 f0A;
    s16 f0C;
    u8 pad0E[3];
    s8 f11;
    s8 f12;
    u8 f13;
} Slot;

typedef struct {
    s16 x;
    s16 y;
    s16 z;
} Vec3;

extern Slot taco_item_pool[];

extern s32 rand(void);

s32 taco_item_add(volatile Vec3 pos, s32 w)
{
    Slot *p;
    s32 i;
    s32 tx, ty, tz;

    p = taco_item_pool;
    for (i = 0; i < 16; i++) {
        if ((p->f13 & 0x80) == 0) {
            break;
        }
        p++;
    }

    if (i < 16) {
        goto found;
    }
    return -1;

found:
    tx = pos.x;
    ty = pos.y;
    tz = pos.z;
    p->f12 = 0;
    p->f13 = 128;
    p->x = tx;
    p->y = ty;
    p->z = tz;
    p->f08 = 63 - (rand() & 0x7F);
    p->f0A = 63 - (rand() & 0x7F);
    p->f0C = 80;
    p->f11 = w;
    return i;
}
