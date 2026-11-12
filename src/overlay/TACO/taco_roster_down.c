/* Address: 0x8014B484 */
/* Size: 316 bytes (79 instructions) */
/* @PostProc: swap_instructions --offset1 0x64 --offset2 0x68 */
/* @PostProc: swap_instructions --offset1 0x84 --offset2 0x8c */
/* @PostProc: swap_instructions --offset1 0x88 --offset2 0x8c */
/* @PostProc: patch_insn_reg_field 0x5c:rt:s0:s1 0x60:rt:s1:s2 0x64:rs:s1:s2 0x64:rt:s1:s2 0x68:rs:s0:s1 0x68:rt:s0:s1 0x6c:rt:s2:s0 0x7c:rs:s1:s2 0x7c:rt:v0:t0 0x80:rs:s1:s2 0x80:rt:v1:t1 0x88:rs:s0:s1 0x8c:rs:s0:s1 0x90:rs:s2:s0 0x94:rt:v0:t0 0x9c:rt:v1:t1 0xa0:rs:s2:s0 0xa0:rt:s2:s0 0xa4:rs:s0:s1 0xa4:rt:s0:s1 0xa8:rs:s2:s0 0xac:rs:s1:s2 0xac:rt:s1:s2 0xb0:rt:t0:t2 0xb4:rs:t0:t2 0xb4:rt:t0:t2 0xb8:rt:t3:t5 0xbc:rs:t0:t2 0xbc:rt:t1:t3 0xc4:rs:t1:t3 0xc4:rt:t2:t4 0xc8:rs:t0:t2 0xc8:rt:t2:t4 0xcc:rs:t3:t5 0xcc:rt:t3:t5 0xd4:rs:t3:t5 0xd8:rt:t4:t7 0xdc:rs:t4:t7 0xdc:rt:t4:t7 0xe0:rt:t7:t8 0xe4:rs:t4:t7 0xe4:rt:t5:t6 0xe8:rt:t1:t3 0xec:rs:t5:t6 0xf4:rs:t4:t7 0xf8:rs:t7:t8 0xf8:rt:t7:t8 0xfc:rt:t9:t1 0x100:rs:t7:t8 0x100:rt:t6:t9 0x104:rt:t8:t0 0x108:rs:t7:t8 0x108:rt:t6:t9 0x10c:rs:t9:t1 0x10c:rt:t9:t1 0x114:rs:t9:t1 0x114:rt:t8:t0 0x118:rs:t1:t3 0x118:rt:t1:t3 0x120:rs:t1:t3 0x120:rt:t2:t4 0x128:rs:t2:t4 0x128:rt:t0:t2 0x12c:rs:t1:t3 0x12c:rt:t0:t2 */
#include "types.h"

typedef struct { s32 w0; s32 w1; } Pair8;
typedef struct { u8 b0; u8 b1; } Pair2;

extern u8 *taco_actor_rec;
extern Pair8 taco_rest_key_tbl[];
extern Pair2 taco_rest_mode_tbl[];

extern void sd_cmd_limit_send();
extern void node_pose_set();

void taco_roster_down(s32 arg) {
    Pair8 *p1;
    Pair2 *p2;
    s32 i;
    u8 b0;
    u8 b1;

    if ((u32)arg < (u32)taco_actor_rec[2315]) {
        if (taco_actor_rec[2314] == 29) {
            sd_cmd_limit_send(0, 1281);
        }
        p1 = taco_rest_key_tbl + 1;
        p2 = taco_rest_mode_tbl + 1;
        for (i = 1; i != 31; i++) {
            b0 = p2[-1].b0;
            b1 = p2[-1].b1;
            node_pose_set(i, taco_actor_rec[2314], p1[-1].w0, p1[-1].w1,
                          b0, b1);
            p1++;
            p2++;
        }
        taco_actor_rec[2314] -= 1;
        taco_actor_rec[2315] = 0;
    }

    if (taco_actor_rec[2314] == 0) {
        taco_actor_rec[2446] = 0;
        taco_actor_rec[2315] = taco_actor_rec[2446];
        taco_actor_rec[2314] = 1;
    } else {
        taco_actor_rec[2315] = taco_actor_rec[2315] + 1;
    }
}
