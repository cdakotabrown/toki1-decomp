/* @CompileFlags: -Wo,-loopunroll,0 -Wc,-pic0 */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x64 */
/* @PostProc: swap_instructions --offset1 0xFC --offset2 0x10C */
/* @PostProc: swap_instructions --offset1 0x108 --offset2 0x110 */
/* @PostProc: remap_stack_slots --from 116 --to 104 */
/* @PostProc: patch_insn_reg_field 0x5c:rt:s3:s2 0x60:rs:s3:s2 0x60:rt:s3:s2 0x6c:rt:s2:s3 0xbc:rt:s2:s3 0xcc:rt:s3:s2 */
/* Address: 0x80147730 */
/* Size: 916 bytes (229 insns) */
#include "types.h"

typedef struct {
    u8  pad00[8];
    s32 f08;
    u8  pad12[8];
    u16 f20;
    u16 f22;
    u16 f24;
    u8  pad26[2];
} Rec28;

typedef struct {
    u8  pad00[24];
    s16 f24;
    u8  pad26[12];
    s16 f38;
    u8  pad40[28];
} Rec68;

typedef struct {
    s32 v[3];
} Cpy12;

extern Rec28 taiiku_spoon_bal[];
extern Rec68 class_w[];
extern Cpy12 taiiku_cpu_steer_tbl;
extern u16 taiiku_spoon_pose_n[];
extern u16 taiiku_spoon_pose_r_2[];
extern u16 taiiku_spoon_pose_l_2[];

extern s32 rand();

void taiiku_cpu_steer(void)
{
    Cpy12 buf;
    Rec28 *p;
    Rec28 *q;
    s32 i;
    s32 j;
    s32 k;
    s32 v;

    buf = taiiku_cpu_steer_tbl;
    p = taiiku_spoon_bal;
    q = &taiiku_spoon_bal[3];
    for (; p != q; p++) {
        if (p[1].f20 > 100U) {
            v = rand() & 0x1F;
            if (v < 10) {
                p[1].f08 = -100;
            } else if (v < 20) {
                p[1].f08 = 100;
            } else {
                p[1].f08 = 0;
            }
            p[1].f20 = 0;
        }
    }
    for (i = 0; i < 3; i++) {
        if (taiiku_spoon_bal[i + 1].f24 > 10U) {
            k = i * 19 + 19;
            for (j = 0; j < 6; j++) {
                if (class_w[97 + i].f24 == taiiku_spoon_pose_r_2[j] + k) {
                    if (class_w[97 + i].f38 + 79 <
                        class_w[101 + i].f38) {
                        if ((rand() & 0xFF) < buf.v[i]) {
                            taiiku_spoon_bal[i + 1].f08 =
                                taiiku_spoon_bal[i + 1].f08 - 34;
                        }
                    }
                    if (class_w[101 + i].f38 <
                        class_w[97 + i].f38 + 64) {
                        if ((rand() & 0xFF) < buf.v[i]) {
                            taiiku_spoon_bal[i + 1].f08 =
                                taiiku_spoon_bal[i + 1].f08 + 34;
                        }
                    }
                }
            }
            for (j = 0; j < 6; j++) {
                if (class_w[97 + i].f24 == taiiku_spoon_pose_l_2[j] + k) {
                    if (class_w[97 + i].f38 + 45 <
                        class_w[101 + i].f38) {
                        if ((rand() & 0xFF) < buf.v[i]) {
                            taiiku_spoon_bal[i + 1].f08 =
                                taiiku_spoon_bal[i + 1].f08 - 34;
                        }
                    }
                    if (class_w[101 + i].f38 <
                        class_w[97 + i].f38 + 40) {
                        if ((rand() & 0xFF) < buf.v[i]) {
                            taiiku_spoon_bal[i + 1].f08 =
                                taiiku_spoon_bal[i + 1].f08 + 34;
                        }
                    }
                }
            }
            for (j = 0; j < 8; j++) {
                if (class_w[97 + i].f24 == taiiku_spoon_pose_n[j] + k) {
                    if (class_w[97 + i].f38 + 45 <
                        class_w[101 + i].f38) {
                        if ((rand() & 0xFF) < buf.v[i]) {
                            taiiku_spoon_bal[i + 1].f08 =
                                taiiku_spoon_bal[i + 1].f08 - 34;
                        }
                    }
                    if (class_w[101 + i].f38 <
                        class_w[97 + i].f38 + 40) {
                        if ((rand() & 0xFF) < buf.v[i]) {
                            taiiku_spoon_bal[i + 1].f08 =
                                taiiku_spoon_bal[i + 1].f08 + 34;
                        }
                    }
                }
            }
            taiiku_spoon_bal[i + 1].f24 = 0;
        }
    }
}
