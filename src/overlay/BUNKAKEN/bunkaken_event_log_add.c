/* @SymbolStart: 0x80145A5C */
/* @SymbolSize: 1060 */
/* @RodataBase: 0x80154C14 */
/* @PostProc: patch_stack_complete --old 32 --new 48 */
/* @PostProc: pad_trailing_nop --count 1 */
/* @PostProc: remap_stack_slots --from 31 --to 43 */
/* @PostProc: swap_instructions --offset1 0x300 --offset2 0x304 */
/* @PostProc: patch_insn_reg_field 0x3e8:rt:v0:t5 0x3f0:rs:v0:t5 0x3f0:rt:v0:t5 0x3f8:rs:v0:t5 0x3f8:rt:v0:t7 0x3fc:rt:v0:t7 0x400:rs:v0:t7 0x400:rt:t5:t9 0x404:rs:t5:t9 0x40c:rt:t7:t1 0x414:rt:t7:t1 */
/* Address: 0x80145A64 (window starts at the absorbed cross-prologue head */
/* Size: 1052 bytes (263 insns) + 8-byte head = 1060 */
#include "types.h"

extern s32 get_g_zyotai_h();

extern u8  screen_w[];
extern u8  now_year;
extern u8  now_month;
extern u8  now_day;
extern u8  now_youbi;
extern u8  girl0_basyo;
extern u8  girl1_basyo;
extern u8  girl2_basyo;
extern u8  girl3_basyo;
extern volatile u8  rec_idx;
extern s32 bunka_bukatu;
extern s32 bunka_girl;

void bunkaken_event_log_add(void) {
    u8 sv;
    s32 nv;
    s32 r;

    switch ((u32)bunka_bukatu) {
        case 0:
            if (0 == ((u32)girl0_basyo >> 4) && bunka_girl == 0) {
                sv = 3;
            } else if (0 == ((u32)girl1_basyo >> 4) && bunka_girl == 1) {
                sv = 3;
            } else {
                sv = 4;
            }
            break;

        case 1:
            if (1 == ((u32)girl0_basyo >> 4) && bunka_girl == 0) {
                sv = 7;
            } else if (1 == ((u32)girl1_basyo >> 4) && bunka_girl == 1) {
                sv = 7;
            } else {
                sv = 8;
            }
            break;

        case 2:
            if (2 == ((u32)girl0_basyo >> 4) && bunka_girl == 0) {
                sv = 11;
            } else if (2 == ((u32)girl2_basyo >> 4) && bunka_girl == 2) {
                sv = 11;
            } else {
                sv = 12;
            }
            break;

        case 3:
            if (3 == ((u32)girl0_basyo >> 4) && bunka_girl == 0) {
                sv = 15;
            } else if (3 == ((u32)girl2_basyo >> 4) && bunka_girl == 2) {
                sv = 15;
            } else {
                sv = 16;
            }
            break;

        case 4:
            if (7 == ((u32)girl0_basyo >> 4) && bunka_girl == 0) {
                sv = 19;
            } else if (7 == ((u32)girl3_basyo >> 4) && bunka_girl == 3) {
                sv = 19;
            } else {
                sv = 20;
            }
            break;

        case 5:
            if (5 == ((u32)girl0_basyo >> 4) && bunka_girl == 0) {
                sv = 23;
            } else if (5 == ((u32)girl3_basyo >> 4) && bunka_girl == 3) {
                sv = 23;
            } else {
                sv = 24;
            }
            break;
    }

    if (bunka_girl == 11) {
        screen_w[rec_idx * 8 + 1886] = 0;
        r = get_g_zyotai_h(0);
        screen_w[rec_idx * 8 + 1889] = (u8)r;
    } else {
        screen_w[rec_idx * 8 + 1886] = (u8)bunka_girl;
        r = get_g_zyotai_h(bunka_girl);
        screen_w[rec_idx * 8 + 1889] = (u8)r;
    }
    screen_w[rec_idx * 8 + 1888] = 20;
    screen_w[rec_idx * 8 + 1887] = sv;
    screen_w[rec_idx * 8 + 1890] = now_year;
    screen_w[rec_idx * 8 + 1891] = now_month;
    screen_w[rec_idx * 8 + 1892] = now_day;
    screen_w[rec_idx * 8 + 1893] = now_youbi;
    nv = rec_idx + 1;
    rec_idx = (u8)nv;
    if ((u32)(nv & 0xFF) >= 255U) {
        rec_idx = 254;
    }
}
