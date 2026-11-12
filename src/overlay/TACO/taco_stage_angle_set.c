/* Address: 0x8014C8AC */
/* Size: 156 bytes (39 insns) */
#include "types.h"

extern s32 taco_stage_ang_tbl[];

extern void angle_set(s32 id, s32 arg);

void taco_stage_angle_set(void)
{
    s32 *p;
    s32 i;

    angle_set(18, taco_stage_ang_tbl[0]);

    p = (&taco_stage_ang_tbl[1]);
    for (i = 0; i < 2; i++) {
        angle_set(i + 25, *p);
        p++;
    }

    angle_set(8, taco_stage_ang_tbl[3]);

    p = (&taco_stage_ang_tbl[4]);
    for (i = 0; i != 2; i++) {
        angle_set(i + 4, *p);
        p++;
    }
}
