/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=21 */
/* @PostProc: scheduler_normalizer */
/* @SymbolSize: 692 */
/* Address: 0x80152D84 */
/* Size: 644 bytes (161 insns) */
#include "types.h"

extern s32 taco_glyph_key_tbl[];

extern void taco_pose_keyframe_ease(s32, s32, s32, s32, s32);
extern void taco_node_shot_spawn(s32, s32, s32, s32, s32, s32, s32);
extern s32 rand(void);

void taco_spin_row_disp(s32 x, s32 idx, s32 mode)
{
    s32 *p;
    s32 i;
    s32 xx;
    s32 rx;
    s32 ry;

    if (mode < 10) {
        i = 0;
        xx = x;
        p = &taco_glyph_key_tbl[idx];
        for (; i != 21; i += 3) {
            taco_pose_keyframe_ease(xx, *p, 8, 0, 0);
            xx += 3;
            p++;
        }
    } else if (mode >= 10 && mode < 20) {
        i = 0;
        xx = x;
        p = &taco_glyph_key_tbl[idx];
        for (; i != 21; i += 3) {
            taco_pose_keyframe_ease(xx, *p, 9, 0, 0);
            xx += 3;
            p++;
        }
    } else if (mode >= 20 && mode < 30) {
        i = 0;
        xx = x;
        p = &taco_glyph_key_tbl[idx];
        for (; i != 21; i += 3) {
            taco_pose_keyframe_ease(xx, *p, 10, 0, 0);
            xx += 3;
            p++;
        }
    } else if (mode >= 30 && mode < 80) {
        rx = rand();
        ry = rand();
        taco_node_shot_spawn(0, 28, (s16)((rx & 0x3FF) - 512),
                      (s16)((ry & 0x3FF) - 512), 1000, 500, 134);
    } else if (mode >= 80 && mode < 90) {
        i = 0;
        xx = x;
        p = &taco_glyph_key_tbl[idx];
        for (; i != 21; i += 3) {
            taco_pose_keyframe_ease(xx, *p, 11, 0, 0);
            xx += 3;
            p++;
        }
    } else if (mode >= 90 && mode < 100) {
        i = 0;
        xx = x;
        p = &taco_glyph_key_tbl[idx];
        for (; i != 21; i += 3) {
            taco_pose_keyframe_ease(xx, *p, 12, 0, 0);
            xx += 3;
            p++;
        }
    } else if (mode >= 100 && mode < 110) {
        i = 0;
        xx = x;
        p = &taco_glyph_key_tbl[idx];
        for (; i != 21; i += 3) {
            taco_pose_keyframe_ease(xx, *p, 13, 0, 0);
            xx += 3;
            p++;
        }
    }
}
