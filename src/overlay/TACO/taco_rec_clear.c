/* Address: 0x80144DA8 */
/* Size: 68 bytes (17 insns) */
#include "types.h"

extern u8 taco_coord[];
extern u8 taco_angle_tbl[];

void taco_rec_clear(s32 n)
{
    u8 *rec;
    s16 *ang;

    rec = taco_coord + n * 80;
    ang = (s16 *)(taco_angle_tbl + n * 8);
    *(s32 *)(rec + 24) = 0;
    *(s32 *)(rec + 28) = 0;
    *(s32 *)(rec + 32) = 0;
    ang[0] = 0;
    ang[1] = 0;
    ang[2] = 0;
}
