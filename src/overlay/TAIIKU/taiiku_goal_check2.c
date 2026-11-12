/* @CompileEnv: IDO52_NO_FINDINDUCT_STRIDE=1 IDO52_LDA_NOINREG_CLASSES=1:1:2,4:1:3,4:5:5,4:7:11 IDO52_NO_LOOP_TEST_REPLACEMENT=1 IDO52_EXCLUDE_REGS_WEB=10:s0+s1+s2+s3+s4+s5+s6+s7+s8 IDO52_AS1_TIE_SRC_ORDER=bb:13 IDO52_AS1_TIE_TAKE=99 IDO52_AS1_TIE_REJECT=116,328,342 */
/* Address: 0x80137F94 */
/* Size: 692 bytes (173 insns) */
/* @PostProc: remap_stack_slots --map '68:52' */
/* @PostProc: swap_instructions --offset1 0x4c --offset2 0x5c */
/* @PostProc: swap_instructions --offset1 0x58 --offset2 0x64 */
/* @PostProc: swap_instructions --offset1 0x64 --offset2 0x68 */
/* @PostProc: swap_instructions --offset1 0xc8 --offset2 0xcc */
/* @PostProc: swap_instructions --offset1 0x288 --offset2 0x298 */
/* @PostProc: swap_instructions --offset1 0x290 --offset2 0x298 */
/* @PostProc: swap_instructions --offset1 0x294 --offset2 0x298 */
/* @PostProc: patch_insn_reg_field 0x24:rt:t1:t0 0x2c:rt:t2:t1 0x34:rt:t1:t0 0x38:rs:t2:t1 0x38:rt:t2:t1 0x3c:rt:t3:t2 0x40:rs:t2:t1 0x48:rs:t3:t2 0x48:rt:t3:t2 0x4c:rt:a0:v0 0x50:rs:t3:t2 0x58:rs:a0:v0 0x58:rt:a0:v0 0x60:rt:a3:a2 0x64:rs:a3:a2 0x64:rt:a3:a2 0x6c:rt:t0:a3 0x70:rt:a2:a1 0x74:rt:a1:a0 0x78:rt:v0:t3 0x7c:rs:a0:v0 0x7c:rt:a1:a0 0x80:rs:v0:a3 0x80:rt:t0:t3 0x84:rs:a0:v0 0x84:rt:a2:a1 0x94:rt:a3:a2 0x98:rs:a0:v0 0x98:rt:a0:v0 0xa0:rt:a3:a2 0xa4:rs:t4:a2 0xa4:rt:a3:t4 0xa8:rt:t0:a3 0xc4:rt:a2:v0 0xc8:rs:a2:v0 0xc8:rt:a2:v0 0xe0:rs:t0:a3 0x100:rs:t6:v0 0x100:rt:a2:t6 0x104:rt:v0:t0 0x10c:rs:v0:t0 0x134:rt:a2:a1 0x138:rs:a2:a1 0x138:rt:a2:a1 0x13c:rt:a0:v0 0x140:rt:a2:a1 0x144:rd:a2:a1 0x148:rs:a0:v0 0x148:rt:a0:v0 0x150:rt:a1:a0 0x154:rt:v0:t4 0x15c:rd:t4:t5 0x15c:rs:v0:t4 0x15c:rt:a2:a1 0x160:rt:a1:a0 0x164:rs:t4:t5 0x164:rt:t5:t9 0x168:rs:a0:v0 0x168:rt:a0:v0 0x170:rs:a0:v0 0x170:rt:t5:t9 0x174:rt:t9:t8 0x178:rs:t9:t8 0x178:rt:t9:t8 0x17c:rt:t6:t7 0x180:rs:t9:t8 0x18c:rt:t8:t6 0x190:rs:t8:t6 0x190:rt:t8:t6 0x198:rs:t8:t6 0x1a4:rs:t6:t7 0x1a4:rt:t6:t7 0x1a8:rt:t7:t1 0x1ac:rs:t6:a2 0x1ac:rt:a3:t7 0x1b4:rs:t7:t1 0x1b4:rt:t7:t1 0x1bc:rs:t7:t1 0x1bc:rt:t1:t0 0x1c0:rt:t1:t0 0x1dc:rt:v1:v0 0x1e0:rs:v1:v0 0x1e0:rt:v1:v0 0x1e8:rs:v1:v0 0x214:rs:v1:v0 0x21c:rt:v1:v0 0x220:rs:v1:v0 0x220:rt:v1:v0 0x224:rs:v1:v0 0x224:rt:v0:t7 0x22c:rs:v0:t7 0x230:rs:v1:v0 0x230:rt:v1:v0 0x23c:rt:t0:a3 0x24c:rt:t7:t1 0x250:rs:t7:t1 0x250:rt:t7:t1 0x254:rt:t7:t1 0x264:rt:t1:t0 0x268:rs:t1:t0 0x268:rt:t1:t0 0x270:rt:t1:t0 0x288:rt:t9:v0 0x2a8:rt:t9:v0 */
#include "types.h"

typedef struct {
    s16 v[4];
} Vec4;

extern Vec4 taiiku_goal_check2_tbl;
extern u8 cw111_anim_flag;
extern u8 class_w[];
extern s16 taiiku_kari_scrl_hi;
extern u8 taiiku_kari_run[];
extern u8 taiiku_kari_run_3_active[];
extern u8 cw112_disp;
extern s16 cw112_frame;
extern u8 cw112_anim_flag;
extern s16 cw112_x;
extern s16 cw114_x;
extern u8 cw114_disp;
extern s16 cw111_x;
extern s16 cal_cw_x;
u8 taiiku_kari_rank[4] = {0};
extern s8 taiiku_kari_gain[];
extern s16 cw113_x;
extern s32 taiiku_konami_mode;

extern s32 get_taiiku_leader_max(void);

s32 taiiku_goal_check2(void)
{
    u8 *cur;
    u8 *pa;
    Vec4 buf;
    s32 a0;
    s32 v0d;
    s32 a1v;

    buf = taiiku_goal_check2_tbl;

    if (cw111_anim_flag == 0 && taiiku_kari_scrl_hi >= 1921) {
        cur = taiiku_kari_run;
        pa = class_w;
        do {
            v0d = *(u8 *)(cur + 42);
            *(u8 *)(pa + 7550) = 64;
            *(u8 *)(pa + 7551) = 164;
            if (v0d == 1) {
                cw112_disp = 0;
            }
            cur += 36;
            pa += 68;
        } while (cur != taiiku_kari_run_3_active);
    }

    if (cw112_frame == 141) {
        cw112_disp = cw112_disp | 0x40;
    }

    cur = taiiku_kari_run;
    pa = class_w;
    for (a0 = 0; a0 < 3; a0++) {
        if (*(u8 *)(cur + 42) == 1) {
            v0d = *(s16 *)(pa + a0 * 68 + 6634);
            if (v0d >= cw112_x + 48) {
                cw112_disp = 0;
                cw112_anim_flag = 0;
            }
        }
        cur += 36;
    }

    a1v = taiiku_kari_scrl_hi * 2;
    pa = class_w;
    cur = (u8 *)&buf.v[0];
    do {
        v0d = *(s16 *)cur;
        cur += 2;
        pa += 68;
        *(s16 *)(pa + 7518) = (s16)(v0d - a1v - 160);
    } while (cur < (u8 *)&buf.v[4]);

    if (cw114_x < -220 && cw114_disp != 0) {
        cw114_disp = 0;
    }

    if (cw112_frame == 141) {
        cw112_x = cw111_x + 32;
    }

    if (cal_cw_x + 64 >= cw111_x) {

        v0d = taiiku_kari_rank[0];
        if (v0d == 0) {
            cw112_disp = 228;
            cw112_frame = 141;
            cw112_x = cw113_x + 32;
        }

        a0 = 0;
        if (v0d != 0) {
            cur = &taiiku_kari_rank[0];
            do {
                v0d = *(u8 *)(cur + 1);
                a0++;
                cur++;
            } while (v0d != 0);
        }

        taiiku_kari_gain[a0 - 4] = 1;
        v0d = a0 + 1;
        if (v0d < 4) {
            cur = &taiiku_kari_rank[v0d];
            do {
                *(u8 *)cur = (u8)get_taiiku_leader_max();
                cur++;
            } while (cur != (u8 *)taiiku_kari_gain);
        }

        if (taiiku_konami_mode == 1U) {
            taiiku_kari_rank[1] = 2;
            taiiku_kari_rank[2] = 4;
            taiiku_kari_rank[3] = 1;
            taiiku_kari_rank[0] = 3;
        }
        return 1;
    }
    return 0;
}
