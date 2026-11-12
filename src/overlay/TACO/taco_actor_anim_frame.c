/* @CompileFlags: -cckr */
/* @PostProc: patch_insn_reg_field 0x9c:rd:s0:s1 0xe0:rs:s0:s1 0xe0:rt:s0:s1 0xf4:rd:s0:s1 0x140:rt:s0:s1 0x148:rt:s0:s1 0x150:rd:s1:s0 0x1a0:rs:s0:s1 0x1c0:rt:s1:s0 0x1ec:rs:s0:s1 0x1f0:rt:s1:s0 0x210:rs:s0:s1 0x210:rt:s0:s1 0x218:rs:s0:s1 */
#include "types.h"

typedef struct {
    u8  pad0[20];
    u8  f20[110];
    s16 f130;
    u8  pad1[2];
    u8  f134;
    u8  pad2[1];
} Rec;

typedef struct {
    u8  b[80];
} Blk80;

extern Rec *taco_actor_rec;
extern u8 taco_node_own[];
extern s32 *taco_kind_pose_tbl[];
extern Blk80 taco_coord[];

taco_actor_anim_frame(idx, hdr)
s32 idx;
s16 *hdr;
{
    s32 i;
    s32 lim;
    s32 n;
    s32 m;
    s32 cnt;

    if (taco_actor_rec[idx].f130 >= hdr[3]) {
        taco_actor_rec[idx].f130 = 0;
    }
    cnt = 0;
    for (i = 0; ; i++) {
        if (taco_actor_rec[idx].f130 % hdr[2] < hdr[i * 2 + 4]) {
            lim = hdr[i * 2 + 5];
            break;
        }
    }
    n = hdr[0];
    m = hdr[1];
    for (i = 0; i < 64; i++) {
        if (idx == taco_node_own[i]) {
            if (cnt >= n) {
                if (taco_actor_rec[idx].f130 % hdr[2] == 0) {
                    GsInitCoordinate2(taco_actor_rec[idx].f20, &taco_coord[i]);
                    taco_rec_pose_set(i, taco_kind_pose_tbl[taco_actor_rec[idx].f134][n]);
                } else {
                    taco_rec_step(i, taco_kind_pose_tbl[taco_actor_rec[idx].f134][n], lim);
                }
                n++;
            }
            cnt++;
        }
        if (m < n) {
            break;
        }
    }
}
