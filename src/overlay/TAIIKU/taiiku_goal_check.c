/* @CompileFlags: -Wo,-loopunroll,0 */
/* @CompileEnv: IDO52_NO_LOOP_TEST_REPLACEMENT=occ:1 */
/* Address: 0x80144610 */
/* Size: 572 bytes (143 insns) */
/* @PostProc: patch_stack_complete --old 64 --new 72 */
/* @PostProc: patch_insn_reg_field 0x1d4:rd:v1:a0 0x1e4:rs:v1:a0 0x1e4:rt:v1:a0 0x1f4:rt:v1:a0 0x1fc:rs:v1:a0 */
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
    u8  pad00;
    u8  f01;
    u8  pad02[34];
} Slot36;

typedef struct {
    s32 w[2];
} Cpy8;

extern Rec68 class_w[];
extern u8 taiiku_ninin_rank[];
extern Slot36 taiiku_ninin_run[];
extern s16 taiiku_ninin_scrl_hi;
extern Cpy8 taiiku_goal_check_tbl;

extern s32 get_taiiku_leader(void);

s32 taiiku_goal_check(void)
{
    s16 buf[4];
    s32 i;
    s32 n;
    u8 *r;

    *(Cpy8 *)buf = taiiku_goal_check_tbl;
    for (i = 0; i < 3; i++) {
        class_w[101 + i].f02 = 64;
        class_w[101 + i].f03 = 164;
    }
    for (i = 0; i < 3; i++) {
        if (taiiku_ninin_run[i + 1].f01 == 1U) {
            if (class_w[101].f38 + 48 < class_w[97 + i].f38 + 64) {
                class_w[102].f03 = 0;
                class_w[102].f02 = 0;
            }
        }
    }
    for (i = 0; i < 4; i++) {
        class_w[101 + i].f38 = (s16)(buf[i] - taiiku_ninin_scrl_hi - 160);
    }
    if (class_w[102].f24 == 50) {
        class_w[102].f03 = (u8)(class_w[102].f03 | 0x40);
        class_w[102].f38 = (s16)(class_w[101].f38 + 32);
    }
    if (class_w[104].f38 < -220 && class_w[104].f03 != 0) {
        class_w[104].f03 = 0;
    }
    if (class_w[96].f38 + 64 >= class_w[101].f38) {
        if (taiiku_ninin_rank[0] == 0) {
            class_w[102].f03 = 228;
            class_w[102].f24 = 50;
            class_w[102].f38 = (s16)(class_w[101].f38 + 32);
        }
        n = 0;
        if (taiiku_ninin_rank[0] != 0) {
            r = taiiku_ninin_rank;
            do {
                n++;
                r++;
            } while (*r != 0);
        }
        taiiku_ninin_rank[n] = 1;
        for (i = n + 1; i < 4; i++) {
            taiiku_ninin_rank[i] = (u8)get_taiiku_leader();
        }
        return 1;
    }
    return 0;
}
