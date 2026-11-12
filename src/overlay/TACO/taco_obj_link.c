/* @PostProc: scheduler_normalizer --block-aware --const-fp --store-fp --load-fp */
/* @PostProc: patch_insn_reg_field 0x1c:rt:s4:s3 0x34:rs:s4:s3 0x38:rs:s4:s3 0x44:rt:s3:s4 0x4c:rs:s3:s4 0x4c:rt:s3:s4 0x58:rt:s4:s3 0x78:rt:s3:s4 0x7c:rt:s4:s3 */
/* Address: 0x80134804 */
/* Size: 144 bytes (36 insns) */
#include "types.h"

extern u8 taco_obj[];
extern u8 taco_coord[];

extern void GsMapModelingData(s32 *p);
extern void GsLinkObject4(s32 *p, s32 *ot, s32 c);

void taco_obj_link(s32 *arg0)
{
    s32 *base;
    s32 *otarg;
    u8 *slot;
    u8 *rec;
    u8 *end;
    s32 fill;

    base = arg0 + 1;
    otarg = base;
    GsMapModelingData(base);

    slot = taco_obj;
    rec = taco_coord;
    end = taco_coord + 5120;
    otarg = otarg + 1;
    otarg = otarg + 1;
    fill = (s32)0x80000000;

    do {
        GsLinkObject4(otarg, (s32 *)slot, 0);
        *(s32 *)(slot + 4) = (s32)rec;
        rec += 80;
        slot += 16;
        *(s32 *)(slot - 16) = fill;
    } while (rec != end);
}
