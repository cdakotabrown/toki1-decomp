/* @PostProc: patch_insn_reg_field 0x18:rt:v1:v0 0x24:rs:v1:v0 0x24:rt:v1:v0 0x2c:rs:v1:v0 0x44:rs:v1:v0 0x5c:rs:v1:v0 0x74:rs:v1:v0 0x90:rs:v1:v0 0xac:rs:v1:v0 0xc8:rs:v1:v0 0xe4:rs:v1:v0 0xe8:rt:v0:t4 0xf0:rs:v0:t4 0xf0:rt:v0:t4 0xf8:rs:v0:t4 0xf8:rt:v0:t1 0xfc:rt:v0:t1 0x100:rs:v0:t1 0x100:rt:t4:t2 0x104:rs:t4:t2 0x10c:rt:t1:t3 0x114:rt:t1:t3 */
/* Address: 0x80138374 */
/* Size: 272 bytes (68 instructions) */
#include "types.h"

extern u8 screen_w[];
extern u8 now_year;
extern u8 now_month;
extern u8 now_day;
extern u8 now_youbi;
extern volatile u8 rec_idx;
extern u8 now_girl;

void master_log_fixed_add(void)
{
    s32 n;

    screen_w[rec_idx * 8 + 1886] = now_girl;
    screen_w[rec_idx * 8 + 1888] = 21;
    screen_w[rec_idx * 8 + 1887] = 6;
    screen_w[rec_idx * 8 + 1889] = 5;
    screen_w[rec_idx * 8 + 1890] = now_year;
    screen_w[rec_idx * 8 + 1891] = now_month;
    screen_w[rec_idx * 8 + 1892] = now_day;
    screen_w[rec_idx * 8 + 1893] = now_youbi;

    n = rec_idx + 1;
    rec_idx = (u8)n;
    if ((u32)(n & 0xFF) >= 255U) {
        rec_idx = 254;
    }
}
