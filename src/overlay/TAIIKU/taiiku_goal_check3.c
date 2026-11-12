/* @CompileFlags: -Wo,-loopunroll,0 */
/* @CompileEnv: IDO52_NO_LOOP_TEST_REPLACEMENT=bb:16 */
/* Address: 0x80146EB4 */
/* Size: 648 bytes (162 insns) */
/* @PostProc: patch_stack_complete --old 72 --new 80 */
/* @PostProc: patch_insn_reg_field 0x1e4:rt:a0:v0 0x1e8:rs:a0:v0 0x1e8:rt:a0:v0 0x1f0:rs:a0:v0 0x21c:rs:a0:v0 0x220:rd:v1:a0 0x230:rs:v1:a0 0x230:rt:v1:a0 0x240:rt:v1:a0 0x248:rs:v1:a0 */
#include "types.h"

typedef struct {
    u8  pad00[2];
    u8  f02;
    u8  f03;
    u8  pad04[20];
    s16 f24;
    u8  pad26[12];
    s16 f38;
    u8  pad40[28];
} Rec68;

typedef struct {
    u8  b00;
    u8  b01;
    u8  pad02[26];
} Slot28;

typedef struct {
    s32 w[2];
} Cpy8;

extern Rec68 class_w[];
extern u8 taiiku_spoon_rank[];
extern s16 taiiku_spoon_scrl_hi;
extern Slot28 taiiku_spoon_cpu[];
extern Cpy8 taiiku_goal_check3_tbl;

extern s32 taiiku_leader_get(void);

s32 taiiku_goal_check3(void)
{
    s16 buf[4];
    s32 i;
    s32 n;
    s32 k;
    s32 v;
    u8 *r;

    *(Cpy8 *)buf = taiiku_goal_check3_tbl;
    if (class_w[104].f02 == 0 && taiiku_spoon_scrl_hi >= 1921) {
        for (i = 0; i < 3; i++) {
            class_w[104 + i].f02 = 64;
            class_w[104 + i].f03 = 164;
            if (taiiku_spoon_cpu[i].b01 == 1U) {
                class_w[105].f03 = 0;
            }
        }
    }
    if (class_w[105].f24 == 79) {
        class_w[105].f03 = (u8)(class_w[105].f03 | 0x40);
    }
    for (i = 0; i < 3; i++) {
        if (taiiku_spoon_cpu[i].b01 == 1U) {
            if (class_w[97 + i].f38 >= class_w[105].f38 + 48) {
                class_w[105].f03 = 0;
                class_w[105].f02 = 0;
            }
        }
    }
    k = taiiku_spoon_scrl_hi;
    k = k * 2;
    for (i = 0; i < 4; i++) {
        class_w[104 + i].f38 = (s16)(buf[i] - k - 160);
    }
    if (class_w[107].f38 < -220 && class_w[107].f03 != 0) {
        class_w[107].f03 = 0;
    }
    if (class_w[105].f24 == 79) {
        class_w[105].f38 = (s16)(class_w[104].f38 + 32);
    }
    if (class_w[96].f38 + 64 >= class_w[104].f38) {
        v = taiiku_spoon_rank[0];
        if (v == 0) {
            class_w[105].f03 = 228;
            class_w[105].f24 = 79;
            class_w[105].f38 = (s16)(class_w[106].f38 + 32);
        }
        n = 0;
        if (v != 0) {
            r = taiiku_spoon_rank;
            do {
                n++;
                r++;
            } while (*r != 0);
        }
        taiiku_spoon_rank[n] = 1;
        for (i = n + 1; i < 4; i++) {
            taiiku_spoon_rank[i] = (u8)taiiku_leader_get();
        }
        return 1;
    }
    return 0;
}
