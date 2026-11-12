/* Address: 0x80152734 */
/* Size: 164 bytes (41 insns) */
#include "types.h"

extern s32 taco_panel_ang_tbl;

extern void taco_pose_keyframe_ease(s32 a0, s32 a1, s32 a2, s32 a3, s32 s16);

void taco_anim_pose_set(s32 arg0)
{
    if (arg0 < 5) {
        taco_pose_keyframe_ease(35, taco_panel_ang_tbl, 1, 0, 0);
    } else if (arg0 >= 5 && arg0 < 15) {
        taco_pose_keyframe_ease(35, taco_panel_ang_tbl, 2, 0, 0);
    } else if (arg0 >= 15 && arg0 < 20) {
        taco_pose_keyframe_ease(35, taco_panel_ang_tbl, 1, 0, 0);
    }
}
