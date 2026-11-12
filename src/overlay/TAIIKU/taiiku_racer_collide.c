/* @SymbolSize: 572 */
/* Address: 0x801480F4 */
/* Size: 572 bytes (143 insns) */
/* @PostProc: patch_stack_complete --old 112 --new 120 */
/* @PostProc: remap_stack_slots --from 96 --to 100 */
/* @PostProc: swap_instructions --offset1 0x84 --offset2 0x88 */
/* @PostProc: swap_instructions --offset1 0x88 --offset2 0x8c */
/* @PostProc: swap_instructions --offset1 0x8c --offset2 0x90 */
/* @PostProc: swap_instructions --offset1 0x90 --offset2 0x94 */
/* @PostProc: swap_instructions --offset1 0x94 --offset2 0x98 */
/* @PostProc: swap_instructions --offset1 0x98 --offset2 0x9c */
/* @PostProc: swap_instructions --offset1 0x9c --offset2 0xa0 */
/* @PostProc: swap_instructions --offset1 0xa0 --offset2 0xa4 */
/* @PostProc: swap_instructions --offset1 0xac --offset2 0xb4 */
/* @PostProc: swap_instructions --offset1 0xb0 --offset2 0xb4 */
/* @PostProc: swap_instructions --offset1 0xb4 --offset2 0xb8 */
/* @PostProc: swap_instructions --offset1 0xb8 --offset2 0xbc */
/* @PostProc: swap_instructions --offset1 0x13c --offset2 0x140 */
/* @PostProc: swap_instructions --offset1 0x1b4 --offset2 0x1b8 */
/* @PostProc: patch_insn_reg_field 0x88:rd:t4:s5 0x8c:rd:t4:s5 0x8c:rs:t4:s5 0xa0:rd:t4:s5 0xa0:rt:t4:s5 0xa4:rt:s1:s2 0xb0:rs:s1:s2 0xb0:rt:s1:s2 0xb4:rs:t4:s5 0xc0:rs:s1:s2 0xc0:rt:t8:t5 0xc4:rt:t5:t4 0xc8:rd:t7:t8 0xc8:rs:t8:t5 0xcc:rs:t5:t4 0xcc:rt:t7:t8 0xd0:rd:s2:s1 0xd4:rs:s2:s1 0xd4:rt:t6:t7 0xd8:rt:t9:t6 0xdc:rt:v1:v0 0xe0:rd:v0:v1 0xe0:rs:t6:t7 0xe0:rt:t9:t6 0xe4:rs:v0:v1 0xe4:rt:t0:t9 0xe8:rs:t0:t9 0xe8:rt:v1:v0 0xf0:rs:v0:v1 0xf0:rt:t3:t2 0xfc:rs:s2:s1 0xfc:rt:t1:t0 0x100:rt:t2:t1 0x104:rt:v1:v0 0x108:rd:v0:v1 0x108:rs:t1:t0 0x108:rt:t2:t1 0x10c:rs:v0:v1 0x10c:rt:t3:t2 0x110:rs:v1:v0 0x110:rt:t3:t2 0x124:rs:s1:s2 0x124:rt:s1:s2 0x128:rs:s1:s2 0x144:rt:t8:t5 0x148:rt:t4:t3 0x14c:rd:t5:t4 0x14c:rs:t8:t5 0x150:rs:t4:t3 0x150:rt:t5:t4 0x158:rt:v0:v1 0x15c:rt:v1:v0 0x160:rs:v0:v1 0x160:rt:t7:t8 0x164:rs:t7:t8 0x164:rt:v1:v0 0x16c:rs:v0:v1 0x16c:rt:t6:t7 0x178:rt:v0:v1 0x17c:rt:v1:v0 0x184:rs:v0:v1 0x184:rt:t6:t7 0x188:rs:v1:v0 0x188:rt:t6:t7 0x1bc:rt:t0:t9 0x1c0:rt:t9:t6 0x1c4:rd:t1:t0 0x1c4:rs:t0:t9 0x1c8:rs:t9:t6 0x1c8:rt:t1:t0 0x1d0:rt:v0:v1 0x1d4:rt:v1:v0 0x1d8:rs:v0:v1 0x1d8:rt:t2:t1 0x1dc:rs:t2:t1 0x1dc:rt:v1:v0 0x1e4:rs:v0:v1 0x1e4:rt:t3:t2 0x1f0:rt:v0:v1 0x1f4:rt:v1:v0 0x1fc:rs:v0:v1 0x1fc:rt:t3:t2 0x200:rs:v1:v0 0x200:rt:t3:t2 */
#include "types.h"

typedef struct {
    s16 v[8];
} Buf801480F4;

extern u8 class_w[];
extern u8 taiiku_spoon_bal_0_down[];
extern u16 taiiku_spoon_pose_n[];
extern u16 taiiku_spoon_pose_r[];
extern u16 taiiku_spoon_pose_l[];
extern u16 taiiku_spoon_pl_tpg[];
extern Buf801480F4 taiiku_racer_collide_tbl;

extern void taiiku_racer_stumble1(s32);
extern void taiiku_racer_stumble2(s32);

void taiiku_racer_collide(s32 idx) {
    Buf801480F4 buf;
    u16 *p;
    s32 j;
    char *elem;
    s32 off;
    s32 sum;
    s32 fld;

    buf = taiiku_racer_collide_tbl;
    if (taiiku_spoon_bal_0_down[idx * 28] != 0) {
        return;
    }
    elem = (char *)class_w + idx * 68;
    off = idx * 20 - idx;

    p = taiiku_spoon_pose_n;
    j = 0;
    do {
        if (*(s16 *)(elem + 6552) == *p + off) {
            sum = *(s16 *)(elem + 6566) + *(s16 *)((char *)buf.v + j);
            fld = *(s16 *)(elem + 6838);
            if (sum + 70 < fld) {
                taiiku_racer_stumble1(idx);
                sum = *(s16 *)(elem + 6566) + *(s16 *)((char *)buf.v + j);
                fld = *(s16 *)(elem + 6838);
            }
            if (fld < sum + 35) {
                taiiku_racer_stumble2(idx);
            }
        }
        p++;
        j += 2;
    } while (p < taiiku_spoon_pose_r);

    p = (&taiiku_spoon_pose_r[2]);
    do {
        if (*(s16 *)(elem + 6552) == *p + off) {
            sum = *(s16 *)(elem + 6566);
            fld = *(s16 *)(elem + 6838);
            if (sum + 94 < fld) {
                taiiku_racer_stumble1(idx);
                sum = *(s16 *)(elem + 6566);
                fld = *(s16 *)(elem + 6838);
            }
            if (fld < sum + 59) {
                taiiku_racer_stumble2(idx);
            }
        }
        p++;
    } while (p < taiiku_spoon_pose_l);

    p = (&taiiku_spoon_pose_l[2]);
    do {
        if (*(s16 *)(elem + 6552) == *p + off) {
            sum = *(s16 *)(elem + 6566);
            fld = *(s16 *)(elem + 6838);
            if (sum + 60 < fld) {
                taiiku_racer_stumble1(idx);
                sum = *(s16 *)(elem + 6566);
                fld = *(s16 *)(elem + 6838);
            }
            if (fld < sum + 25) {
                taiiku_racer_stumble2(idx);
            }
        }
        p++;
    } while (p != taiiku_spoon_pl_tpg);
}
