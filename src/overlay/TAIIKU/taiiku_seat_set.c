/* Address: 0x80132E20 */
/* Size: 692 bytes (173 insns) */
/* @PostProc: patch_stack_complete --old 184 --new 192 */
/* @PostProc: remap_stack_slots --map '112:120,160:168' */
/* @PostProc: patch_insn_reg_field 0x1b4:rt:v1:v0 0x1b8:rs:v1:v0 0x1b8:rt:v1:v0 0x1c0:rs:v1:v0 0x1cc:rs:v1:v0 0x210:rt:a2:a1 0x214:rs:a2:a1 0x214:rt:a2:a1 0x218:rt:a1:a0 0x21c:rt:a0:v1 0x220:rt:v1:v0 0x228:rs:v1:v0 0x230:rs:v1:v0 0x23c:rt:a0:v1 0x244:rt:a1:a0 0x274:rt:a0:v1 0x27c:rt:a1:a0 0x294:rt:a2:a1 */
#include "types.h"

typedef struct {
    u8  pad00[2];
    u8  f02;
    u8  f03;
    u8  pad04[4];
    s16 f08;
    u8  pad10[14];
    s16 f24;
    u8  pad26[12];
    s16 f38;
    u8  pad40[28];
} Rec68;

typedef struct {
    s16 v[23];
} Tbl46;

extern Rec68 class_w[];
extern Tbl46 taiiku_seat_x;

extern s32 abs();

void taiiku_seat_set(s16 arg0, s32 arg1)
{
    s16 work[12];
    Tbl46 tbl;
    s32 i;
    s32 j;
    s32 v;
    s32 best;

    tbl = taiiku_seat_x;
    for (i = 0; i < 23; i++) {
        for (j = 0; j < arg1; j++) {
            work[j] = (s16)(tbl.v[i] + j * 1280 - arg0 - 160);
        }
        for (j = 0; j < arg1; j++) {
            best = abs(work[j]);
            if (best < abs(work[0])) {
                work[0] = work[j];
            }
        }
        class_w[i + 1].f38 = work[0];
    }
    for (i = 0; i < 23; i++) {
        v = class_w[i + 1].f38;
        if (v < 177 && v >= -295) {
            class_w[i + 1].f02 = 1;
            class_w[i + 1].f03 = 164;
        } else {
            class_w[i + 1].f02 = 0;
            class_w[i + 1].f03 = 0;
            class_w[i + 1].f24 = 0;
            class_w[i + 1].f08 = 0;
        }
    }
}
