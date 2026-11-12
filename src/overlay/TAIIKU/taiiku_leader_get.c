/* Address: 0x8014713C */
/* Size: 200 bytes (50 insns) */
/* @CompileFlags: -Wo,-loopunroll,0 -Wc,-pic0 */
/* @PostProc: swap_instructions --offset1 0x1c --offset2 0x2c */
/* @PostProc: swap_instructions --offset1 0x20 --offset2 0x2c */
/* @PostProc: swap_instructions --offset1 0x24 --offset2 0x2c */
/* @PostProc: swap_instructions --offset1 0x28 --offset2 0x2c */
/* @PostProc: swap_instructions --offset1 0x2c --offset2 0x30 */
/* @PostProc: patch_insn_reg_field 0x14:rt:v0:v1 0x18:rt:a3:a1 0x1c:rs:a3:a1 0x1c:rt:a3:a1 0x20:rs:v0:v1 0x20:rt:v0:v1 0x24:rt:v1:a0 0x28:rd:a0:v0 0x30:rt:a1:a3 0x38:rs:v0:v1 0x3c:rs:v0:v1 0x3c:rt:v0:v1 0x48:rs:a0:v0 0x50:rs:t7:a1 0x50:rt:a3:t7 0x5c:rs:v1:a0 0x60:rs:v1:a0 0x64:rs:a0:v0 0x64:rt:a0:v0 0x68:rs:a0:v0 0x68:rt:a1:a3 0x6c:rs:v1:a0 0x6c:rt:v1:a0 */
#include "types.h"

extern u8 taiiku_spoon_cpu[];
extern s16 class_w[];
extern u8 taiiku_spoon_cpu_0_b01;
extern u8 taiiku_spoon_cpu_1_b01;
extern u8 taiiku_spoon_cpu_2_b01;

s32 taiiku_leader_get(void) {
    s32 unused;
    s16 buf[3];
    s32 i;
    u8 *p;
    s16 *d;
    s32 lock;
    s32 n;
    s32 stride;
    char *src;

    p = taiiku_spoon_cpu;
    src = (char *)class_w;
    d = buf;
    i = 0;
    lock = -3840;
    n = 3;
    stride = 68;
    do {
        if (p[1] == 0) {
            *d = *(s16 *)(src + i * stride + 6634);
        } else {
            *d = (s16)lock;
        }
        p += 28;
        i++;
        d++;
    } while (i != n);

    if (buf[0] >= buf[1] && buf[0] >= buf[2]) {
        taiiku_spoon_cpu_0_b01 = 1;
        return 2;
    }
    if (buf[1] >= buf[2]) {
        taiiku_spoon_cpu_1_b01 = 1;
        return 3;
    }
    taiiku_spoon_cpu_2_b01 = 1;
    return 4;
}
