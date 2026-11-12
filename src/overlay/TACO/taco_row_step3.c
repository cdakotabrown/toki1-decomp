/* @PostProc: swap_instructions --offset1 0xb4 --offset2 0xbc */
/* @PostProc: patch_insn_reg_field 0x94:rt:t9:t8 0xc8:rt:v0:t7 0xcc:rt:v1:t6 0xe0:rt:v0:t7 0xe8:rt:v1:t6 0xfc:rt:t7:t9 0x100:rt:t6:t0 0x104:rs:t6:t0 0x104:rt:t6:t0 0x108:rs:t7:t9 0x108:rt:t7:t9 0x124:rt:t6:t0 0x12c:rt:t7:t9 0x138:rs:t9:t8 0x138:rt:t9:t8 0x140:rs:t9:t8 0x140:rt:t0:t1 0x148:rs:t0:t1 0x148:rt:t8:t3 0x14c:rs:t9:t8 0x14c:rt:t8:t3 */
/* Address: 0x80150DB8 */
/* Size: 348 bytes (87 insns) */
#include "types.h"

typedef struct { s32 w0; s32 w1; } Pair8;
typedef struct { u8 b0; u8 b1; } Pair2;

extern u8 *taco_actor_rec;
extern Pair8 taco_node_pose_tbl[];
extern Pair2 taco_node_mode_tbl_m1[];

extern void node_pose_set(s32, s32, s32, s32, s32, s32);
extern void taco_row_disp3(s32);

void taco_row_step3(void)
{
    s32 i;
    Pair2 *p2;
    Pair8 *p1;
    s32 v;
    s32 b0;
    s32 b1;

    if ((u32)taco_actor_rec[2587] >= 5U) {
        taco_row_disp3(taco_actor_rec[2586]);
        taco_actor_rec[2586] = taco_actor_rec[2586] + 1;
        taco_actor_rec[2587] = 0;
    }

    if ((u32)taco_actor_rec[2586] >= 100U) {
        taco_actor_rec[2718] = 0;
        v = taco_actor_rec[2718];
        taco_actor_rec[2586] = v;
        taco_actor_rec[2587] = v;
        i = 7;
        p1 = taco_node_pose_tbl + 8;
        p2 = taco_node_mode_tbl_m1 + 8;
        do {
            b0 = p2[-1].b0;
            b1 = p2[-1].b1;
            node_pose_set(i, 0, p1[-1].w0, p1[-1].w1, b0, b1);
            i++;
            p1++;
            p2++;
        } while (i != 13);
        node_pose_set(27, 0, taco_node_pose_tbl[27].w0, taco_node_pose_tbl[27].w1,
                      taco_node_mode_tbl_m1[27].b0, taco_node_mode_tbl_m1[27].b1);
    } else {
        taco_actor_rec[2587] = taco_actor_rec[2587] + 1;
    }
}
