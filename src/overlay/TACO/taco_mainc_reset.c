/* @PostProc: swap_instructions --offset1 0x50 --offset2 0x54 */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x64 */
/* @PostProc: patch_insn_reg_field 0x50:rt:s3:s4 0x54:rt:s2:s3 0x58:rt:s4:s2 0x5c:rs:s4:s2 0x5c:rt:s4:s2 0x60:rs:s2:s3 0x60:rt:s2:s3 0x64:rs:s3:s4 0x64:rt:s3:s4 0x84:rs:s4:s2 0x88:rs:s2:s3 0x90:rt:s3:s4 */
/* Address: 0x80149B4C */
/* Size: 416 bytes (104 insns) */
#include "types.h"

extern u8 *taco_actor_rec;
extern u8 taco_mainc_kind_tbl[];
extern u8 taco_mainc_rad_tbl[];
extern u8 taco_mainc_hp_tbl[];

extern void taco_object_slot_init(s32);
extern void taco_actor_slot_set(s32, s32, s32, s32, s32, s32);

void taco_mainc_reset(void)
{
    s32 i;

    for (i = 16; i < 63; i++) {
        taco_object_slot_init(i);
    }

    for (i = 0; i < 12; i++) {
        taco_actor_slot_set(taco_mainc_kind_tbl[i], i, 0x80196000, (s32)taco_mainc_rad_tbl,
                      (s32)taco_mainc_hp_tbl, 1);
    }

    taco_actor_rec[2310] = 0;
    taco_actor_rec[2179] = 0;
    taco_actor_rec[2178] = 0;
    taco_actor_rec[2446] = 0;
    taco_actor_rec[2315] = 0;
    taco_actor_rec[2314] = 0;
    taco_actor_rec[2582] = 1;
    taco_actor_rec[2451] = 0;
    taco_actor_rec[2450] = 0;
    taco_actor_rec[2718] = 1;
    taco_actor_rec[2587] = 0;
    taco_actor_rec[2586] = 0;
    taco_actor_rec[2854] = 1;
    taco_actor_rec[2723] = 0;
    taco_actor_rec[2722] = 0;
    taco_actor_rec[2859] = 0;
    taco_actor_rec[2858] = 0;
    taco_actor_rec[2995] = 0;
    taco_actor_rec[2994] = 0;
}
