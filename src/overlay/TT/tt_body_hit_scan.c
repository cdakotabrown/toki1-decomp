/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=1,2,128 */
/* @PostProc: patch_stack_complete --old 104 --new 152 */
/* @PostProc: remap_stack_slots --from 100 --to 108 --from 102 --to 110 */
/* @PostProc: swap_instructions --offset1 0x80 --offset2 0x84 */
/* @PostProc: swap_instructions --offset1 0x84 --offset2 0x88 */
/* @PostProc: swap_instructions --offset1 0x8c --offset2 0x94 */
/* @PostProc: swap_instructions --offset1 0x90 --offset2 0x94 */
/* @PostProc: patch_insn_reg_field 0x4c:rt:s3:s2 0x60:rs:s3:s2 0x60:rt:s3:s2 0x74:rt:s5:s6 0x78:rt:s7:fp 0x80:rs:s7:fp 0x80:rt:s7:fp 0x84:rs:s5:s6 0x84:rt:s5:s6 0x88:rd:s2:s3 0x8c:rt:fp:s4 0x90:rt:s4:s5 0x94:rt:s6:s7 0xb0:rs:s4:s5 0xb4:rs:s5:s6 0xb8:rs:s6:s7 0xc0:rs:s7:fp 0xdc:rs:s3:s2 0xdc:rt:v0:t7 0xe0:rs:s3:s2 0xe0:rt:t7:t9 0xe4:rs:v0:t7 0xe4:rt:v0:t8 0xe8:rs:t7:t9 0xec:rs:s3:s2 0xec:rt:v0:t8 0xf0:rs:v0:t8 0xf0:rt:v1:v0 0xf4:rs:v1:v0 0xf4:rt:t8:t0 0xf8:rs:t8:t0 0xfc:rs:v1:v0 0xfc:rt:t9:t1 0x100:rs:t9:t1 0x110:rt:t0:t2 0x114:rt:t0:t2 0x134:rt:t1:t6 0x138:rt:t1:t6 0x14c:rs:s2:s3 0x158:rt:t2:t3 0x160:rs:t2:t3 0x160:rt:t6:t4 0x168:rt:t6:t4 0x16c:rt:t3:t5 0x174:rs:t3:t5 0x17c:rt:t4:t7 0x184:rs:t4:t7 0x184:rt:t5:t8 0x18c:rt:t5:t8 0x190:rt:t7:t9 0x198:rs:t7:t9 0x198:rt:t8:t0 0x1a0:rt:t8:t0 0x1ac:rt:t9:t1 0x1b0:rt:t9:t1 0x1b4:rt:t0:t2 0x1b8:rt:t0:t2 0x1d4:rs:s2:s3 0x1d4:rt:s2:s3 0x1d8:rs:s2:s3 0x1d8:rt:fp:s4 */
/* tt_body_hit_scan - TT overlay, Address: 0x80137878, Size: 512 bytes (128 insns) */
#include "types.h"
#include "tokimeki/tt_box.h"

extern u8 *tt_boss_wk;
extern u8 *tt_step_wk;
extern u8 *tt_player_shot_buf;
extern TT_BOX tt_hit_box_tbl[];
extern TT_BOX tt_target_box_tbl[];

extern s32 tt_box_overlap_check(u8 *a0, u8 *a1, s16 *a2, u8 *a3);
extern void tt_cmd_once(s32 a0);
extern void tt_shot_spread_make(s16 x, s16 y, s32 spread, s32 mul, u16 kind);
extern void tt_shot_fan4_make(s16 x, s16 y, s32 spread, s32 mul, u16 kind);

void tt_body_hit_scan(void)
{
    s16 buf[2];
    u8 *p;
    u8 *q;
    u8 *e;
    s32 i;
    s32 c;
    s32 v;

    p = tt_boss_wk;
    buf[0] = *(s16 *)(p + 20) + *(s16 *)(p + 596);
    q = tt_step_wk;
    buf[1] = *(s16 *)(p + 22) + *(s16 *)(p + 644);
    p[186] = 0;
    e = tt_player_shot_buf;
    i = 0;
    do {
        if (e[0] != 0) {
            if (*(u16 *)(e + 4) == 64) {
                if (tt_box_overlap_check(e + 20, ((u8 *)&tt_hit_box_tbl[1].x), buf,
                                  ((u8 *)&tt_target_box_tbl[4].x)) != 0) {
                    if (*(u16 *)(p + 782) == 2) {
                        c = q[37] + 1;
                        q[37] = c;
                        if (q[36] != 0) {
                            v = (u8)c;
                            if ((v & 3) == 0) {
                                if ((v & 7) == 0) {
                                    tt_shot_fan4_make(*(s16 *)(e + 20),
                                                  *(s16 *)(e + 22),
                                                  256, 32, 160);
                                } else {
                                    tt_shot_spread_make(*(s16 *)(e + 20),
                                                  *(s16 *)(e + 22),
                                                  256, 32, 160);
                                }
                            }
                        }
                        tt_cmd_once(1287);
                        if (i >= 6) {
                            *(s16 *)(p + 780) = *(s16 *)(p + 780) - 1;
                        } else if (*(u16 *)(e + 2) == 9) {
                            *(s16 *)(p + 780) = *(s16 *)(p + 780) - 4;
                        } else {
                            *(s16 *)(p + 780) = *(s16 *)(p + 780) - 2;
                        }
                    } else {
                        tt_cmd_once(1286);
                    }
                    p[186] = 1;
                    *(s16 *)(e + 4) = 128;
                    e[13] = 0;
                    e[14] = 0;
                    e[15] = 0;
                    *(s16 *)(e + 6) = 0;
                    *(s16 *)(e + 8) = 0;
                    *(s16 *)(e + 10) = 0;
                }
            }
        }
        i++;
        e += 84;
    } while (i != 12);
}
