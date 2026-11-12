/* @PostProc: patch_stack_complete --old 48 --new 72 */
/* @PostProc: remap_stack_slots --from 40 --to 44 --from 36 --to 56 --from 28 --to 60 */
/* @PostProc: swap_instructions --offset1 0x58 --offset2 0x5c */
/* Address: 0x8014040C */
/* Size: 316 bytes (79 instructions) */
#include "types.h"

typedef struct {
    s16 x;
    s16 y;
    s16 z;
    s16 unk6;
    s16 unk8;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    u8 unk16;
    u8 unk17;
    u8 unk18;
    u8 flags;
} TacoSlot;

typedef struct {
    s16 x;
    s16 y;
    s16 z;
    s16 w;
} TacoVec;

extern u8 *taco_actor_rec;
extern TacoSlot taco_actor_buf[];
extern TacoSlot taco_actor2_buf[];

extern void sd_cmd_limit_send(s32 arg0, s32 arg1);
extern void taco_target_find(s32 *out, TacoVec v);

s32 taco_actor_chase_spawn(s32 arg0, s32 arg1)
{
    TacoSlot *base;
    TacoSlot *p;
    u8 *rec;
    s32 i;
    s32 out;

    if (arg0 == 0) {
        rec = taco_actor_rec;
        base = taco_actor_buf;
    } else {
        rec = taco_actor_rec + 1088;
        base = taco_actor2_buf;
    }

    if (arg1 & 0x20) {
        p = base;
        i = 0;
        do {
            if (!(p->flags & 0x80)) {
                break;
            }
            i += 20;
            p += 1;
        } while (i != 320);

        if (!(i < 320)) {
            return -1;
        }

        if (arg0 == 0) {
            sd_cmd_limit_send(1, 1283);
        }

        p->flags = 145;
        p->x = *(s16 *)(rec + 116);
        p->y = *(s16 *)(rec + 118) + 64;
        p->z = *(s16 *)(rec + 120) + 64;
        p->unk8 = 0;
        p->unk10 = 0;
        p->unk12 = -256;
        p->unk18 = 0;
        taco_target_find(&out, *(TacoVec *)p);
        p->unk17 = out;
        return 1;
    }
    return 0;
}
