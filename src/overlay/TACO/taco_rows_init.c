/* Address: 0x8014A76C */
/* Size: 324 bytes (81 insns) */
#include "types.h"

extern u8 taco_main16_kind_tbl[];
extern u8 taco_main16_rad_tbl[];
extern u8 taco_main16_hp_tbl[];

extern u8 *taco_actor_rec;

extern void taco_actor_slot_set(s32 a0, s32 a1, s32 a2, s32 a3, s32 s16, s32 s20);

void taco_rows_init(void)
{
    s32 i;
    s32 cst;
    u8 *b;
    u8 *c;

    cst = 0x801A7000;
    b = taco_main16_rad_tbl;
    c = taco_main16_hp_tbl;
    for (i = 0; i != 11; i++) {
        taco_actor_slot_set(taco_main16_kind_tbl[i], i, cst, (s32)b, (s32)c, 1);
    }

    taco_actor_rec[2179] = 0;
    taco_actor_rec[2178] = 0;
    taco_actor_rec[2315] = 0;
    taco_actor_rec[2314] = 0;
    taco_actor_rec[2582] = 0;
    taco_actor_rec[2451] = 0;
    taco_actor_rec[2450] = 0;
    taco_actor_rec[2718] = 0;
    taco_actor_rec[2587] = 0;
    taco_actor_rec[2586] = 0;
    taco_actor_rec[2854] = 0;
    taco_actor_rec[2722] = 0;
    taco_actor_rec[2859] = 0;
    taco_actor_rec[2858] = 0;
}
