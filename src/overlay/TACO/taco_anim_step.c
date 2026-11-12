/* @SymbolStart: 0x8015263C */
/* @SymbolSize: 248 */
/* @PostProc: patch_insn_reg_field 0x90:rt:t6:t7 0x98:rt:t7:t6 0xa0:rs:t7:t6 0xa0:rt:t7:t6 0xa4:rs:t6:t7 0xa4:rt:t6:t7 0xbc:rt:t7:t6 0xc4:rt:t6:t7 */
/* Address: 0x80152644 */
#include "types.h"

extern u8 *taco_actor_rec;
extern s32 taco_panel_ang_tbl;
extern s32 taco_panel_key_tbl[][2];
extern u8 taco_panel_mode_tbl[][2];

extern void node_pose_set(s32, s32, s32, s32, s32, s32);
extern void angle_set(s32, s32);
extern void taco_anim_pose_set(s32);

void
taco_anim_step(s32 count)
{
    u8 *p;
    u8 *g;

    p = taco_actor_rec;

    if ((u32)count < (u8)p[2315]) {
        taco_anim_pose_set(p[2314]);
        g = taco_actor_rec;
        g[2314] = (u8)g[2314] + 1;
        g = taco_actor_rec;
        g[2315] = 0;
    }
    g = taco_actor_rec;
    if ((u8)g[2314] >= 20U) {
        g[2314] = 0;
        g = taco_actor_rec;
        g[2315] = (u8)g[2314];
        node_pose_set(35, 0, taco_panel_key_tbl[34][0], taco_panel_key_tbl[34][1],
                      (u8)taco_panel_mode_tbl[34][0], (u8)taco_panel_mode_tbl[34][1]);
        angle_set(35, taco_panel_ang_tbl);
    } else {
        g = taco_actor_rec;
        g[2315] = (u8)g[2315] + 1;
    }
}
