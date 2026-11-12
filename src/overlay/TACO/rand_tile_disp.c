/* @PostProc: swap_instructions --offset1 0x28 --offset2 0x2c */
/* @PostProc: patch_insn_reg_field 0x2c:rd:v0:v1 0x30:rt:v1:t6 0x34:rs:v0:v1 0x34:rt:v0:v1 0x38:rs:v1:t6 0x3c:rs:v0:v1 0x3c:rt:a0:at 0x50:rd:t6:a3 0x54:rt:t6:a3 0x68:rt:t8:a3 0x70:rd:t9:t7 0x70:rt:t8:a3 0x74:rd:t7:t8 0x78:rd:v1:t9 0x78:rt:t9:t8 0x80:rt:v1:t9 0x84:rs:a0:at 0xa4:rt:v0:v1 0xac:rs:v0:v1 0xb0:rt:v0:v1 0xb4:rs:v0:v1 0xb4:rt:v1:v0 0xb8:rt:v1:v0 0xc4:rs:v1:v0 */
/* Address: 0x8013507C */
#include "types.h"

extern s32 screen_w[];
extern s32 girl1_flag[];
extern s32 frame_counter;

extern s32 rand();
extern void taco_tile_disp();

void
rand_tile_disp(void)
{
    s32 *cur;
    s32 v1;
    s32 hi[1];
    s32 a3;
    s32 v0;
    s32 t6;
    s32 t9;
    s32 at;

    v1 = 0;
    cur = screen_w;
    do {
        t6 = cur[1551];
        v1 += 1;
        at = (v1 < 128);
        if (t6 == 0) {
            hi[0] = rand() % 320;
            t9 = (hi[0] << 16) + (rand() % 240);
            cur[1551] = t9;
            break;
        }
        cur += 1;
    } while (at);

    for (cur = &girl1_flag[-128]; cur != girl1_flag; cur++) {
        v1 = cur[1551];
        if (v1 != 0) {
            a3 = (u32)v1 >> 16;
            v0 = v1 & 0xFFFF;
            taco_tile_disp(a3 - 160, v0 - 120,
                          (a3 * v0 + frame_counter) & 0x00FFFFFF, a3);
        }
    }
}
