/* @SymbolStart: 0x8013718C */
/* @SymbolSize: 408 */
/* @CompileFlags: -Wo,-loopunroll,0 -Wc,-pic0 */
/* @PostProc: swap_instructions --offset1 0x158 --offset2 0x15c */
/* @PostProc: patch_insn_reg_field 0x58:rt:a0:v1 0x68:rs:a0:v1 0x68:rt:a0:v1 0x7c:rs:a0:v1 0x7c:rt:v1:a0 0x84:rs:v1:t2 0x84:rt:t2:a0 0xb8:rs:a0:v1 0xc4:rt:a2:t8 0xc8:rs:a0:v1 0xc8:rt:a3:a2 0xd0:rs:a3:a2 0xd0:rt:a2:t8 0xd8:rs:a3:a2 0xdc:rs:a0:v1 0xdc:rt:t8:t7 0xe4:rs:t8:t7 0xe8:rs:a3:a2 0xec:rs:a0:v1 0xf4:rs:a0:v1 0xf8:rs:a3:a2 0x104:rs:a0:v1 0x104:rt:t7:t9 0x108:rd:t9:t6 0x10c:rs:t7:t9 0x110:rd:t6:t8 0x110:rt:t9:t6 0x114:rs:t6:t8 0x114:rt:t8:t7 0x11c:rd:t7:t9 0x11c:rt:t8:t7 0x120:rs:t7:t9 0x120:rt:t9:t6 0x128:rs:a0:v1 0x128:rt:t9:t6 0x12c:rs:a0:v1 0x12c:rt:t6:t8 0x134:rs:t6:t8 0x140:rs:a0:v1 0x140:rt:t8:t7 0x148:rs:t8:t7 0x150:rs:v1:a0 0x154:rd:t6:t7 0x158:rs:a0:v1 0x15c:rs:a0:v1 0x15c:rt:t7:t6 0x160:rd:v1:a0 0x160:rt:t6:t7 0x164:rd:a2:a3 0x164:rs:t7:t9 0x164:rt:t9:t6 0x168:rs:a2:a3 0x16c:rs:a0:v1 0x16c:rt:a2:a3 0x170:rs:a0:v1 0x174:rd:a2:a3 0x178:rs:v1:a0 0x178:rt:a3:a2 0x180:rs:a3:a2 0x180:rt:a2:a3 0x18c:rs:a0:v1 0x18c:rt:a3:a2 0x190:rs:a0:v1 0x19c:rs:a0:v1 0x19c:rt:a0:v1 */
#include "types.h"

typedef struct { s32 w[3]; } Rec12;
typedef struct { s16 h[3]; } Rec6;

extern Rec12 taiiku_racer_speed_set_tbl0;
extern Rec6 taiiku_racer_speed_set_tbl1;
extern s16 cw111_x;
extern u8 class_w[][68];
extern u8 taiiku_kari_run[][36];

void taiiku_racer_speed_set(void) {
    s32 pad1;
    s32 pad2;
    Rec12 buf12;
    Rec6 buf6;
    s32 i;
    s32 sum;
    s32 v;
    s32 flag;
    s32 a2v;
    s32 one;
    s32 h;

    one = 1;
    buf12 = taiiku_racer_speed_set_tbl0;
    buf6 = taiiku_racer_speed_set_tbl1;

    for (i = 0; i != 3; i++) {
        flag = *(u8 *)(taiiku_kari_run[i] + 42);
        if (flag == one) {
            if ((cw111_x + i * 8 + 328) <
                *(s16 *)(class_w[i] + 6634)) {
                *(s32 *)(taiiku_kari_run[i] + 60) = 0;
                continue;
            }
        }
        h = buf6.h[i];
        a2v = *(s16 *)(taiiku_kari_run[i] + 70);
        if (!(a2v < h) && *(u8 *)(taiiku_kari_run[i] + 36) == 0) {
            *(s32 *)(taiiku_kari_run[i] + 60) = 0;
            *(u8 *)(taiiku_kari_run[i] + 37) = (u8)one;
            continue;
        }
        if (!(a2v < 320) && *(u8 *)(taiiku_kari_run[i] + 36) == 0) {
            *(s32 *)(taiiku_kari_run[i] + 60) = (buf12.w[i] >> 1) + 1;
            continue;
        }
        if (*(u32 *)(taiiku_kari_run[i] + 64) < 11U) {
            continue;
        }
        if (*(u8 *)(taiiku_kari_run[i] + 37) != 0) {
            continue;
        }
        if (flag != 0) {
            continue;
        }
        sum = *(s32 *)(taiiku_kari_run[i] + 60) + *(s32 *)(taiiku_kari_run[i] + 52);
        *(s32 *)(taiiku_kari_run[i] + 60) = sum;
        if (sum < 0) {
            *(s32 *)(taiiku_kari_run[i] + 60) = 0;
            sum = 0;
        }
        v = buf12.w[i];
        if (v < sum) {
            *(s32 *)(taiiku_kari_run[i] + 60) = v;
        }
        *(s32 *)(taiiku_kari_run[i] + 64) = 0;
    }
}
