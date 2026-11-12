/* Address: 0x8014A3FC */
/* Size: 84 bytes (21 insns) */
#include "types.h"

extern void taco_actor_set2(s32 a, s32 b, void *c, void *d, void *e, s32 f);
extern void taco_drift_step(void);

extern char taco_main16_rad_tbl[];
extern char taco_main16_hp_tbl[];
extern char taco_main16_ofs_tbl[];
extern u8 taco_main16_kind_tbl[];

void taco_main16_drift_step(void)
{
    taco_actor_set2(taco_main16_kind_tbl[1], 1, taco_main16_ofs_tbl, taco_main16_rad_tbl, taco_main16_hp_tbl, 1);
    taco_drift_step();
}
