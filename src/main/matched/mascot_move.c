/* Function: mascot_move  (radio / tuning-meter update).  The o.bin name */
/* Address: 0x800679A0  (true start; 0x800679A8 is the PROLOGUE) */
/* Size: 952 bytes (238 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* The shipped state carried a whole-function `@PostProc: scheduler_normalizer` */
/* 6. `@CompileFlags: -Wc,-pic1` -- the main binary is not wired up for the */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=81:a1 IDO52_COPYPROP_SITE=lod:6:deny IDO52_UGEN_TEMP_REG=18:t4,19:t5,25:t9,26:t3 IDO52_AS1_TIE_REJECT=330,358 */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @SymbolStart: 0x800679A0 */
/* @SymbolSize: 952 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern void mascot_init();
extern s32 rsin();
extern s32 SquareRoot0();
extern void line_set(s16, s16, s16, s16, s32, s32, s32);

extern s16 cursor_work_copy_tbl0;
extern u32 pad_trig;
extern s32 frame_counter;
extern u32 pad_data;
extern u8 mascot_hi;
extern u8 mascot_cw;
extern s16 mascot_x;
extern s16 mascot_y;
extern s32 mascot_cw_timing;
extern s16 mascot_cw_x;
extern s16 mascot_cw_y;
extern s16 mascot_cw_frame;
extern s16 mascot2_cw_x;
extern s16 mascot2_cw_y;
extern s16 mascot2_cw_frame;

void mascot_move()
{
    s32 unused;
    s16 s;
    s32 v;
    s32 x;
    s16 d;
    u32 pad;

    if (cursor_work_copy_tbl0 == 0 || cursor_work_copy_tbl0 >= 14) {
        mascot_init(0);
        return;
    }
    mascot_init(cursor_work_copy_tbl0);
    if (cursor_work_copy_tbl0 >= 14) {
        return;
    }
    if (pad_trig >> 16) {
        mascot_cw = (mascot_hi + 1) >> 1;
    }
    if ((frame_counter & 0x3F) == 63) {
        if (mascot_cw != 0) {
            mascot_cw--;
        }
    }
    v = rsin((frame_counter & 0x7F) << 5);
    s = (v * mascot_cw) >> 12;
    mascot_cw_timing = ((-s * 2) / 3) << 12;
    v = SquareRoot0(mascot_hi * mascot_hi - s * s);
    x = mascot_x + s;
    d = 15 - v;
    mascot_cw_x = x;
    mascot_cw_y = mascot_y + mascot_hi - d;
    mascot_cw_frame = cursor_work_copy_tbl0 - 1;
    pad = pad_data;
    mascot2_cw_x = mascot_x;
    mascot2_cw_y = mascot_y;
    if (pad & 2) {
        mascot2_cw_frame = 14;
        switch (pad & 0x5000) {
        case 0x1000:
            if (mascot_hi >= 3) {
                mascot_hi -= 2;
                mascot_cw = (mascot_hi + 1) >> 1;
            }
            break;
        case 0x4000:
            if (mascot_hi < 196) {
                mascot_hi += 2;
                mascot_cw = (mascot_hi + 1) >> 1;
            }
            break;
        }
    } else if (pad & 8) {
        mascot2_cw_frame = 14;
        switch (pad & 0x5000) {
        case 0x1000:
            mascot_y -= 4;
            break;
        case 0x4000:
            mascot_y += 4;
            break;
        }
        switch (pad & 0xA000) {
        case 0x8000:
            mascot_x -= 4;
            x -= 4;
            break;
        case 0x2000:
            mascot_x += 4;
            x += 4;
            break;
        }
    } else {
        mascot2_cw_frame = 13;
    }
    line_set(mascot_x, mascot_y, x, mascot_y + mascot_hi - d, 0xF8B0C8, 0xF8B0C8, 1);
    if (mascot_x >= 513) {
        mascot_x = -512;
    }
    if (mascot_x < -512) {
        mascot_x = 512;
    }
    if (mascot_y >= 513) {
        mascot_y = -512;
    }
    if (mascot_y < -512) {
        mascot_y = 512;
    }
}
