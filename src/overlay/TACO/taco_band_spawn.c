/* @CompileEnv: IDO52_UGEN_NO_ARG_COALESCE=1 IDO52_FORCE_WEB_COLOR=4:s2,7:s1 */
/* @PostProc: swap_instructions --offset1 0x4c --offset2 0x58 */
/* @PostProc: swap_instructions --offset1 0x4c --offset2 0x50 */
/* @PostProc: swap_instructions --offset1 0x9c --offset2 0xa8 */
/* @PostProc: swap_instructions --offset1 0x9c --offset2 0xa0 */
/* Address: 0x8014BDE4 */
/* Size: 196 bytes (49 insns) */
#include "types.h"
extern s32 taco_step_ang_tbl[];


extern void taco_pose_keyframe_ease(s32 a0, s32 a1, s32 a2, s32 a3, s32 s16);

void taco_band_spawn(s32 arg0)
{
    s32 *p;
    s32 i;
    s32 n;

    if (arg0 < 8) {
        p = (&taco_step_ang_tbl[6]);
        i = 0;
        do {
            n = i + 1;
            taco_pose_keyframe_ease(n, *p, 1, 0, 0);
            i = n;
            p++;
        } while (n != 3);
    } else if (arg0 >= 28 && arg0 < 36) {
        i = 0;
        p = (&taco_step_ang_tbl[6]);
        do {
            n = i + 1;
            taco_pose_keyframe_ease(n, *p, 2, 0, 0);
            i = n;
            p++;
        } while (n != 3);
    }
}
