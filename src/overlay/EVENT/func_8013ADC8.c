/* Address: 0x8013ADC8 */
/* Size: 428 bytes (107 instructions) */
/* @PostProc: patch_insn_reg_field 0x34:rt:v0:a0 0x3c:rs:v0:a0 0x3c:rt:v0:a0 0x48:rs:v0:a0 0x54:rd:v0:a0 0x64:rs:v0:a0 0x6c:rd:a0:v0 0x70:rd:v0:a0 0x80:rs:v0:a0 0x88:rd:v0:a0 0xf8:rd:v0:v1 0x104:rs:v0:v1 0x108:rt:v0:v1 0x10c:rs:v0:v1 0x118:rd:v0:v1 0x134:rs:v0:v1 0x148:rs:v0:v1 0x14c:rd:v1:v0 0x14c:rt:v0:v1 0x150:rs:v0:v1 0x154:rd:v1:v0 0x158:rs:v1:v0 0x170:rs:v1:v0 0x188:rs:v1:v0 0x19c:rs:v1:v0 */
#include "types.h"

extern s16 D_800F19D0[4];
extern s16 D_800EAFB8;
extern s32 D_800B1AE4;

extern s32 func_80015A64(void);
extern void func_800458E0(void);

void func_8013ADC8(void)
{
    s32 best;
    s32 bestIdx;
    s32 second;
    s16 i;
    s32 total;
    s32 rank;

    if (func_80015A64() & 4) {
        func_800458E0();
    }

    best = D_800F19D0[0];
    bestIdx = 0;
    if (best < D_800F19D0[1]) {
        best = D_800F19D0[1];
        bestIdx = 1;
    }
    if (best < D_800F19D0[2]) {
        best = D_800F19D0[2];
        bestIdx = 2;
    }
    if (best < D_800F19D0[3]) {
        best = D_800F19D0[3];
        bestIdx = 3;
    }

    second = 0;
    for (i = 0; i < 4; i++) {
        if (i == bestIdx) {
            continue;
        }
        if (second < D_800F19D0[i]) {
            second = D_800F19D0[i];
        }
    }

    total = D_800F19D0[0] + D_800F19D0[1] + D_800F19D0[2] + D_800F19D0[3];
    total = total / 4;

    if (D_800EAFB8 != 0) {
        if ((D_800B1AE4 & 3) != 0) {
            return;
        }
    }

    if (total == 0) {
        D_800EAFB8 = 3;
        return;
    }

    rank = total / 16;
    if (rank == 0) {
        D_800EAFB8 = 3;
    } else if (rank == 1) {
        D_800EAFB8 = 2;
    } else if (rank == 2) {
        D_800EAFB8 = 1;
    } else if (rank >= 3) {
        D_800EAFB8 = 0;
    }
}
