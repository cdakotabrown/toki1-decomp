/* Address: 0x801464C4 */
/* Size: 288 bytes (72 instructions) */
/* @CompileEnv: IDO52_EXCLUDE_REGS_WEB=18:a0 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x1c */
/* @PostProc: patch_insn_reg_field 0x18:rt:v0:a2 0x1c:rt:v1:a1 0x20:rt:a1:s0 0x24:rt:a2:v1 0x28:rs:v0:a1 0x28:rt:v0:a1 0x2c:rs:v1:a2 0x2c:rt:v1:a2 0x30:rs:a1:v1 0x30:rt:a1:v1 0x34:rs:a2:s0 0x34:rt:a2:s0 0x38:rt:s0:t5 0x3c:rt:t5:t4 0x40:rt:t4:t3 0x44:rt:t3:t2 0x48:rt:t2:t1 0x4c:rt:t1:t0 0x50:rt:t0:a3 0x54:rs:a1:v1 0x54:rt:a3:v0 0x5c:rs:a3:v0 0x64:rs:v1:a2 0x64:rt:a3:v0 0x6c:rs:a1:v1 0x74:rs:v0:a1 0x7c:rs:a1:v1 0x7c:rt:a3:v0 0x84:rs:a3:v0 0x8c:rs:a1:v1 0x94:rs:t0:a3 0x9c:rt:t1:t0 0xac:rs:a1:v1 0xc4:rt:t2:t1 0xcc:rs:a3:t2 0xcc:rt:t3:v0 0xd4:rs:a1:v1 0xdc:rs:t4:t3 0xec:rt:t5:t4 0xf4:rs:a3:t5 0xf4:rt:s0:v0 0xfc:rs:a1:v1 0x104:rs:t0:a3 0x108:rs:a1:v1 0x108:rt:a1:v1 0x114:rs:a1:v1 0x114:rt:a1:v1 0x118:rs:a1:v1 0x118:rt:a2:s0 */
#include "types.h"

extern u8 now_year;
extern u8 now_month;
extern u8 screen_w[];

s32 etc_month_record_check(s32 a0) {
    u8 *p;
    u8 *end;
    s32 a2;
    s32 a1;
    s32 v0;

    a1 = now_month;
    a2 = now_year;
    p = screen_w;
    end = &screen_w[2048];

    do {
        v0 = p[1890];
        if (v0 == 0) {
            goto ret0;
        }
        if (a2 == v0 && a1 == p[1891]) {
            v0 = p[1888];
            if (v0 == 0 && 15 == p[1887]) {
                if (a0 == 15U) {
                    return 1;
                }
                if (a0 == p[1886]) {
                    return 1;
                }
            }
            if (a0 == 8 && 2 == v0 && 59 == p[1887]) {
                return 9;
            }
            if (a0 == 11 && 11U == v0) {
                if (15 == p[1887]) {
                    return 1;
                }
            }
        }
        p += 8;
    } while (p != end);

ret0:
    return 0;
}
