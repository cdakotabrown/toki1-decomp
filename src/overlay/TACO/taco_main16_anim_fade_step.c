/* Address: 0x8014A360 */
/* Size: 176 bytes (44 insns) */
#include "types.h"

extern u8 taco_main16_kind_tbl[];
extern u8 taco_main16_rad_tbl[];
extern u8 taco_main16_hp_tbl[];
extern u8 taco_main16_ofs_tbl[];

extern void taco_actor_set2(s32 a0, s32 a1, s32 a2, s32 a3, s32 s16, s32 s20);
extern void taco_anim_fade_step(void);

void taco_main16_anim_fade_step(void)
{
    s32 i;

    for (i = 0; i != 11; i++) {
        taco_actor_set2(taco_main16_kind_tbl[i], i, (s32)taco_main16_ofs_tbl,
                      (s32)taco_main16_rad_tbl, (s32)taco_main16_hp_tbl, 0);
    }

    taco_anim_fade_step();
}
