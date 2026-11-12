/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=21 */
/* @PostProc: patch_stack_complete --old 72 --new 144 */
/* @PostProc: remap_stack_slots --from 56 --to 76 */
/* @PostProc: scheduler_normalizer */
/* @SymbolSize: 668 */
/* Address: 0x801523A0 */
/* Size: 656 bytes (164 insns) */
#include "types.h"

extern s32 taco_glyph_key_tbl[];
extern u8 *taco_actor_rec;

extern void taco_pose_keyframe_ease(s32, s32, s32, s32, s32);
extern void sd_cmd_limit_send(s32, s32);
extern void taco_node_shot_spawn(s32, s32, s32, s32, s32, s32, s32);

void taco_reel_row_disp(s32 arg0, s32 idx, s32 mode)
{
    s32 *p;
    s32 i;
    s32 x;
    volatile s32 rec;

    if (mode < 8) {
        i = 0;
        x = arg0;
        p = &taco_glyph_key_tbl[idx];
        for (; i != 21; i += 3) {
            taco_pose_keyframe_ease(x, *p, 3, 0, 0);
            x += 3;
            p++;
        }
    } else if (mode >= 8 && mode < 16) {
        i = 0;
        x = arg0;
        p = &taco_glyph_key_tbl[idx];
        for (; i != 21; i += 3) {
            taco_pose_keyframe_ease(x, *p, 4, 0, 0);
            x += 3;
            p++;
        }
    } else if (mode >= 16 && mode < 26) {
        i = 0;
        x = arg0;
        p = &taco_glyph_key_tbl[idx];
        for (; i != 21; i += 3) {
            taco_pose_keyframe_ease(x, *p, 5, 0, 0);
            x += 3;
            p++;
        }
    } else if (mode == 29) {
        sd_cmd_limit_send(0, 1281);
    } else if (mode >= 30 && mode < 35) {
        i = 0;
        x = arg0;
        p = &taco_glyph_key_tbl[idx];
        for (; i != 21; i += 3) {
            taco_pose_keyframe_ease(x, *p, 6, 0, 0);
            x += 3;
            p++;
        }
        if (arg0 == 1) {
            rec = 16;
        } else if (arg0 == 2) {
            rec = 17;
        } else if (arg0 == 3) {
            rec = 18;
        }
        if (taco_actor_rec[rec * 136 + 133] != 0) {
            taco_node_shot_spawn(0, arg0, 0, 100, 1000, 200, 131);
        }
    } else if (mode >= 35 && mode < 45) {
        i = 0;
        x = arg0;
        p = &taco_glyph_key_tbl[idx];
        for (; i != 21; i += 3) {
            taco_pose_keyframe_ease(x, *p, 7, 0, 0);
            x += 3;
            p++;
        }
    }
}
