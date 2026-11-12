/* @SymbolStart: 0x80150C6C */
/* @SymbolSize: 548 */
/* Function: date_line_fan_disp */
/* Overlay: DATE */
/* @PostProc: patch_insn_reg_field 0x20:rt:a0:a1 0x24:rs:a0:a1 0x24:rt:a0:a1 0x74:rs:a0:a1 0xbc:rd:t2:t1 0xc0:rs:t2:t1 0xf4:rt:t1:t0 0x11c:rt:t2:t1 0x120:rt:t2:t1 0x128:rt:t1:t0 0x130:rd:t3:t2 0x134:rd:t4:t3 0x134:rt:t3:t2 0x13c:rs:t4:t3 0x16c:rd:t5:t4 0x174:rd:t6:t5 0x174:rt:t5:t4 0x178:rs:t6:t5 0x1a4:rd:t7:t6 0x1a8:rd:t8:t7 0x1a8:rt:t7:t6 0x1b0:rs:t8:t7 0x1e0:rd:t9:t8 0x1e8:rt:t9:t8 0x1f0:rd:t3:t2 0x1f4:rt:t3:t2 0x200:rd:t2:t1 */
#include "types.h"

extern u8 fade_bri;
extern u32 frame_counter;

extern s32 rsin(s32);
extern s32 rcos(s32);
extern void line_set(s32, s32, s32, s32, s32, s32, s32);

void date_line_fan_disp(void)
{
    s32 base;
    u32 col;
    u32 col2;
    s16 i;

    col = (fade_bri / 10U) * 19;
    col = (col << 16) | (col << 8) | col;
    col2 = col;

    base = 0;
    if (frame_counter % 45 < 15) {
        base = 170;
    } else if (frame_counter % 45 < 30) {
        base = 341;
    }

    for (i = 0; i < 16; i++) {
        line_set((s16)(rcos((i << 8) + base) / 20),
                      (s16)(rsin((i << 8) + base) / 20 - 30),
                      (s16)(rcos((i << 8) + base) / 60),
                      (s16)(rsin((i << 8) + base) / 60 - 30),
                      col, col2, 10);
    }
}
