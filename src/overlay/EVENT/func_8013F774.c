/* @PostProc: patch_insn_reg_field 0x118:rt:t8:t6 0x11c:rt:t6:t7 0x120:rd:v1:t0 0x120:rs:t6:t7 0x120:rt:t8:t6 0x124:rt:t2:t3 0x128:rd:t7:t8 0x12c:rs:v1:t0 0x12c:rt:t7:t8 0x138:rt:t0:t1 0x13c:rt:t2:t3 0x140:rs:t0:t1 0x140:rt:t1:t2 0x144:rt:t1:t2 0x160:rt:t3:t4 0x168:rt:t3:t4 0x170:rt:t4:t5 0x178:rs:t4:t5 0x198:rt:t5:t9 0x1a0:rs:t5:t9 0x1a8:rt:t9:t7 0x1b0:rt:t9:t7 0x1c8:rd:t3:t4 0x1d0:rt:t7:t0 0x1d8:rd:t1:t2 0x1d8:rs:t7:t0 0x1e0:rt:t1:t2 0x1e4:rd:t4:t5 0x1e4:rt:t3:t4 0x1e8:rs:t4:t5 0x1e8:rt:t4:t5 0x1ec:rt:t2:t3 0x1f0:rs:t4:t5 0x1f0:rt:t5:t9 0x1f4:rs:t5:t9 0x1f4:rt:t2:t3 0x200:rt:t9:t7 0x204:rt:t7:t0 0x208:rs:t9:t7 0x214:rd:t7:t0 0x214:rs:t7:t0 0x218:rs:t7:t0 0x218:rt:t7:t0 0x21c:rd:t1:t2 0x220:rs:t7:t0 0x220:rt:t0:t1 0x224:rt:t0:t1 0x238:rs:t1:t2 0x240:rd:t3:t4 0x244:rt:t3:t4 */
/* Address: 0x8013F774 */
/* Size: 628 bytes (157 instructions) */
#include "types.h"

typedef struct {
    s16 x;
    s16 y;
} Vec2;

typedef struct {
    Vec2 p[8];
} Vec2Tbl;

extern Vec2Tbl D_80122990;
extern s16 D_80122764[];
extern u8 D_8012274C[];
extern u8 D_80122774[];
extern u8 D_800EAFA7;
extern u8 D_800E9620[];

extern s32 func_800789A0(void);

void func_8013F774(void)
{
    s16 i;
    u32 rest;
    Vec2Tbl home;
    u8 *p;
    s32 r;

    home = D_80122990;

    for (i = 0; i < 8; i++) {
        p = &D_800E9620[i * 68];
        p[6671] = D_800EAFA7;
        if (D_8012274C[i] == 0) {
            D_80122774[i]++;
        }
        if (D_8012274C[i] == 0) {
            if (D_80122774[i] >= 121U) {
                r = func_800789A0() % 60;
                rest = 180 - D_80122774[i];
                if (rest < (u32)r) {
                    p[6667] |= 0x80;
                    p[6666] = 5;
                    D_8012274C[i] = 1U;
                }
            }
        }

        if (i == 0) {
            if ((u32)D_8012274C[i] == 1U && *(s16 *)(p + 6688) == 7 &&
                *(s16 *)(p + 6672) == 0) {
                *(s32 *)(p + 6712) = 0;
                D_8012274C[i] = 2;
            }
        } else {
            if ((u32)D_8012274C[i] == 1U && *(s16 *)(p + 6688) == 3 &&
                *(s16 *)(p + 6672) == 7) {
                *(s32 *)(p + 6712) = 0;
                D_8012274C[i] = 2;
            }
        }

        if (D_8012274C[i] == 2U) {
            *(s32 *)(p + 6712) += 16384;
            *(s32 *)(p + 6704) += *(s32 *)(p + 6712);
            if (home.p[i].y - 120 < *(s16 *)(p + 6706)) {
                p[6667] &= 0x7F;
                p[6666] = 0;
                *(s16 *)(p + 6706) = D_80122764[i] - 120;
                *(s16 *)(p + 6704) = 0;
                *(s16 *)(p + 6688) = 0;
                *(s16 *)(p + 6672) = 0;
                D_8012274C[i] = 0;
                D_80122774[i] = 0;
            }
        }
    }
}
