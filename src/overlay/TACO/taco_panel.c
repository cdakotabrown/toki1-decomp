/* @PostProc: scheduler_normalizer --block-aware */
/* @PostProc: patch_insn_reg_field 0x84:rt:t3:t4 0x88:rt:v0:t3 0x90:rs:v0:t3 0x98:rt:t7:t9 0xa0:rs:t3:t4 0xa0:rt:t3:t4 0xa8:rs:t3:t4 0xa8:rt:t4:t5 0xc4:rs:t3:t4 0xc4:rt:t4:t5 0xc8:rt:t5:t7 0xe0:rt:t5:t7 0x114:rs:t7:t9 0x114:rt:t7:t9 0x11c:rs:t7:t9 0x11c:rt:t9:t0 0x124:rs:t9:t0 0x124:rt:t0:t8 0x128:rs:t7:t9 0x128:rt:t0:t8 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* taco_panel - TACO overlay, Address: 0x801527D8, Size: 316 bytes (79 */
#include "types.h"

extern u8 *taco_actor_rec;
extern s32 taco_panel_key_tbl[][2];
extern u8 taco_panel_mode_tbl[][2];
extern s32 taco_panel_ang_tbl[];

extern void taco_band_row_disp(s32 a0, s32 a1);
extern void node_pose_set(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern void angle_set(s32 a0, s32 a1);

void taco_panel(s32 arg0)
{
    u8 *p0;
    u8 *p1;
    u8 *p2;
    u8 *p3;
    u8 *p4;
    u8 *p5;
    s32 *q1;
    u8 *q2;
    s32 *q3;
    s32 s0;
    s32 t3;

    p0 = taco_actor_rec;
    if ((u32)arg0 < (u32)p0[2859]) {
        taco_band_row_disp(29, p0[2858]);
        p1 = taco_actor_rec;
        p1[2858] = p1[2858] + 1;
        p2 = taco_actor_rec;
        p2[2859] = 0;
    }
    p3 = taco_actor_rec;
    t3 = p3[2858];
    if ((u32)t3 >= 70U) {
        s0 = 29;
        p3[2858] = 0;
        p4 = taco_actor_rec;
        p4[2859] = p4[2858];
        q1 = &(&taco_panel_key_tbl[28][0])[2];
        q2 = &(&taco_panel_mode_tbl[28][0])[2];
        q3 = &(&taco_panel_ang_tbl[1])[0];
        for (; s0 != 33; s0++) {
            node_pose_set(s0, 0, q1[-2], q1[-1], q2[-2], q2[-1]);
            angle_set(s0, q3[0]);
            q1 += 2;
            q2 += 2;
            q3 += 1;
        }
    } else {
        p5 = taco_actor_rec;
        p5[2859] = p5[2859] + 1;
    }
}
