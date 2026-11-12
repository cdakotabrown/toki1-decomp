/* @PostProc: swap_instructions --offset1 0x5c --offset2 0x60 */
/* @PostProc: swap_instructions --offset1 0x6c --offset2 0x70 */
/* @PostProc: patch_insn_reg_field 0x5c:rt:s3:s4 0x60:rt:s2:s3 0x64:rt:s4:s2 0x68:rs:s4:s2 0x68:rt:s4:s2 0x6c:rs:s2:s3 0x6c:rt:s2:s3 0x70:rs:s3:s4 0x70:rt:s3:s4 0x8c:rs:s4:s2 0x90:rs:s2:s3 0x98:rt:s3:s4 */
/* Address: 0x80148B28 */
/* Size: 532 bytes (133 insns) */
#include "types.h"

extern u8 *taco_actor_rec;
extern u8 taco_main2_kind_tbl[];
extern u8 taco_main2_rad_tbl[];
extern u8 taco_main2_hp_tbl[];

extern void taco_object_slot_init(s32);
extern void taco_actor_slot_set(s32, s32, s32, s32, s32, s32);

void taco_stage_reset(s32 arg)
{
    s32 i;

    for (i = 16; i < 63; i++) {
        taco_object_slot_init(i);
    }

    if (arg > 0) {
        for (i = 0; i < 11; i++) {
            taco_actor_slot_set(taco_main2_kind_tbl[i], i, 0x8018D000, (s32)taco_main2_rad_tbl,
                          (s32)taco_main2_hp_tbl, 2);
        }
    }

    taco_actor_rec[2310] = 0;
    taco_actor_rec[2179] = 0;
    taco_actor_rec[2178] = 0;
    taco_actor_rec[2446] = 0;
    taco_actor_rec[2315] = 0;
    taco_actor_rec[2314] = 1;
    taco_actor_rec[2582] = 0;
    taco_actor_rec[2451] = 0;
    taco_actor_rec[2450] = 0;
    taco_actor_rec[2718] = 0;
    taco_actor_rec[2587] = 0;
    taco_actor_rec[2586] = 0;
    taco_actor_rec[2854] = 0;
    taco_actor_rec[2723] = 0;
    taco_actor_rec[2722] = 0;
    taco_actor_rec[2990] = 0;
    taco_actor_rec[2859] = 0;
    taco_actor_rec[2858] = 0;
    taco_actor_rec[3126] = 0;
    taco_actor_rec[2995] = 0;
    taco_actor_rec[2994] = 0;
    taco_actor_rec[3262] = 0;
    taco_actor_rec[3398] = 0;
    taco_actor_rec[3267] = 0;
    taco_actor_rec[3266] = 0;
    taco_actor_rec[3534] = 0;
    taco_actor_rec[3403] = 0;
    taco_actor_rec[3402] = 0;
}
