/* @PostProc: patch_insn_reg_field 0x9c:rt:v0:v1 0xa4:rs:v0:v1 0xc8:rt:v0:v1 0xd8:rs:v0:v1 */
/* Address: 0x80151CEC */
/* Size: 632 bytes (158 insns) */
#include "types.h"

extern u8 *taco_actor_rec;
extern s32 taco_panel_key_tbl[][2];
extern u8 taco_panel_mode_tbl[];

extern void sd_cmd_limit_send(s32, s32);
extern s32 rand(void);
extern void taco_node_shot_spawn(s32, s32, s32, s32, s32, s32, s32);
extern void node_pose_set(s32, s32, s32, s32, s32, s32);

void taco_panel_anim(u32 arg)
{
    u32 v;
    s32 x;
    s32 y;
    s32 i;

    if (arg < (u32)taco_actor_rec[2179]) {
        for (i = 22; i < 28; i++) {
            node_pose_set(i, taco_actor_rec[2178], taco_panel_key_tbl[i - 1][0],
                          taco_panel_key_tbl[i - 1][1], taco_panel_mode_tbl[(i - 1) * 2],
                          taco_panel_mode_tbl[(i - 1) * 2 + 1]);
        }
        v = taco_actor_rec[2178];
        if (v % 10U == 0) {
            sd_cmd_limit_send(0, 1282);
            v = taco_actor_rec[2178];
        }
        taco_actor_rec[2178] = (u8)(v + 1);
        taco_actor_rec[2179] = 0;
        if ((u32)taco_actor_rec[2178] % 3U == 0) {
            x = rand();
            y = rand();
            taco_node_shot_spawn(0, 22, (s16)((x & 0x1FF) - 256),
                          (s16)((y & 0x1FF) - 256), 1000, 200, 196);
            x = rand();
            y = rand();
            taco_node_shot_spawn(0, 23, (s16)((x & 0x1FF) - 256),
                          (s16)((y & 0x1FF) - 256), 1000, 200, 196);
        }
    }

    if ((u32)taco_actor_rec[2178] >= 30U) {
        taco_actor_rec[2178] = 0;
        taco_actor_rec[2179] = taco_actor_rec[2178];
        for (i = 22; i < 28; i++) {
            node_pose_set(i, 0, taco_panel_key_tbl[i - 1][0], taco_panel_key_tbl[i - 1][1],
                          taco_panel_mode_tbl[(i - 1) * 2],
                          taco_panel_mode_tbl[(i - 1) * 2 + 1]);
        }
        if (taco_actor_rec[2446] == 1U) {
            taco_actor_rec[2310] = 1;
        }
    } else {
        taco_actor_rec[2179] = (u8)(taco_actor_rec[2179] + 1);
    }
}
