/* @SymbolSize: 456 */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x18 */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x64 */
/* @PostProc: swap_instructions --offset1 0x64 --offset2 0x68 */
/* @PostProc: swap_instructions --offset1 0x6c --offset2 0x70 */
/* @PostProc: swap_instructions --offset1 0x70 --offset2 0x74 */
/* @PostProc: swap_instructions --offset1 0xd0 --offset2 0xf0 */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x24 */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x2c */
/* @PostProc: swap_instructions --offset1 0x24 --offset2 0x28 */
/* @PostProc: swap_instructions --offset1 0x28 --offset2 0x2c */
/* @PostProc: swap_instructions --offset1 0x30 --offset2 0x38 */
/* @PostProc: swap_instructions --offset1 0x44 --offset2 0x48 */
/* @PostProc: swap_instructions --offset1 0x48 --offset2 0x4c */
/* @PostProc: patch_insn_reg_field 0x34:rt:s2:s1 0x38:rt:s1:s3 0x3c:rt:s4:s6 0x44:rs:s2:s6 0x44:rt:s2:s6 0x48:rs:s4:s3 0x48:rt:s4:s3 0x54:rt:s3:s2 0x58:rt:s5:s4 0x5c:rt:s6:s5 0x8c:rt:s3:s2 0xc4:rt:s4:s3 0xd4:rt:s5:s4 0xdc:rs:t0:s0 0xdc:rt:s0:t0 0x104:rt:s6:s5 0x108:rt:v1:t8 0x10c:rs:v1:t8 0x10c:rt:v1:t8 0x118:rd:t8:t6 0x118:rt:v1:t8 0x11c:rd:t6:t9 0x11c:rt:t8:t6 0x120:rt:a0:t1 0x124:rs:t6:t9 0x128:rs:a0:t1 0x128:rt:a0:t1 0x12c:rt:t9:t0 0x130:rs:t9:t0 0x130:rt:t9:t0 0x134:rd:t0:t2 0x134:rt:a0:t1 0x138:rd:t1:t3 0x138:rt:t0:t2 0x13c:rt:a1:t5 0x140:rs:t1:t3 0x140:rt:t9:t0 0x144:rs:a1:t5 0x144:rt:a1:t5 0x148:rt:t2:t4 0x14c:rs:t2:t4 0x14c:rt:t2:t4 0x150:rd:t3:t8 0x150:rt:a1:t5 0x154:rd:t4:t6 0x154:rt:t3:t8 0x158:rt:a2:t9 0x15c:rs:t4:t6 0x15c:rt:t2:t4 0x160:rs:a2:t9 0x160:rt:a2:t9 0x164:rt:t5:t7 0x168:rs:t5:t7 0x168:rt:t5:t7 0x16c:rd:t8:t1 0x16c:rt:a2:t9 0x170:rd:t7:t2 0x170:rt:t8:t1 0x174:rt:v0:t0 0x178:rs:t7:t2 0x178:rt:t5:t7 0x17c:rs:v0:t0 0x17c:rt:v0:t0 0x184:rs:v0:t0 0x184:rt:v0:t3 0x188:rt:v0:t3 0x18c:rs:v0:t3 0x18c:rt:t6:t5 0x190:rs:t6:t5 0x19c:rt:t0:t8 0x1a4:rt:t0:t8 0x1a8:rt:s2:s6 */
/* Address: 0x8013296C */
/* Size: 456 bytes (114 insns) */
/* owed there).  `@SymbolSize: 456` trims the compare window to the real body: */
#include "types.h"

extern s32 get_g_zyotai_h(s32 a0);

extern u8 screen_w[];
extern volatile u8 taiiku_girl_ord[];
extern volatile u8 rec_idx;
extern volatile u8 now_year;
extern volatile u8 now_month;
extern volatile u8 now_day;
extern volatile u8 now_youbi;

void taiiku_rireki_set(void) {
    u8 *s0;
    volatile u8 *s1;
    volatile u8 *s3;
    volatile u8 *s6;
    s32 s2 = 19;
    s32 s4 = 56;
    s32 s5 = 4;
    s32 v0;
    s32 t3;
    u32 t5;
    s32 i0;
    s32 i1;
    s32 i2;
    s32 i3;

    s0 = screen_w;
    s1 = taiiku_girl_ord;
    s3 = taiiku_girl_ord + 2;
    s6 = taiiku_girl_ord + 3;

    do {
        s0[(rec_idx << 3) + 1886] = *s1;
        s0[(rec_idx << 3) + 1888] = (u8)s2;
        s0[(rec_idx << 3) + 1887] = 0;
        v0 = get_g_zyotai_h(*s1);
        s0[(rec_idx << 3) + 1889] = (u8)v0;
        if (s1 == s3) {
            if (*(s16 *)(s0 + (*s1) * s4 + 454) >= 76) {
                s0[(rec_idx << 3) + 1889] = (u8)s5;
            }
        }
        i0 = rec_idx;
        s0[(i0 << 3) + 1890] = now_year;
        i1 = rec_idx;
        s0[(i1 << 3) + 1891] = now_month;
        i2 = rec_idx;
        s0[(i2 << 3) + 1892] = now_day;
        i3 = rec_idx;
        s0[(i3 << 3) + 1893] = now_youbi;
        t3 = rec_idx + 1;
        rec_idx = (u8)t3;
        t5 = t3 & 0xFF;
        if (t5 >= 255) {
            rec_idx = 254;
        }
        s1++;
    } while (s1 != s6);
}
