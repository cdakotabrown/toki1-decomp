/* @CompileEnv: IDO52_UGEN_NO_ARG_COALESCE=1 IDO52_FORCE_WEB_COLOR=0:s1,3:s0,9:s2 */
/* Address: 0x8014BEA8 */
#include "types.h"
extern s32 taco_step_ang_tbl[];


extern void angle_set(s32, s32);

void taco_band_angle_init(void)
{
    s32 *ptr;
    s32 i;
    s32 idx;
    s32 limit;

    ptr = &taco_step_ang_tbl[6];
    i = 0;
    limit = 3;
    do {
        idx = i + 1;
        angle_set(idx, *ptr);
        i = idx;
        ptr++;
    } while (idx != limit);
}
