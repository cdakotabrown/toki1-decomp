/* Address: 0x8007C968 */
/* Size: 2732 bytes (683 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_FORCE_WEB_NOSPILL=2 IDO52_FORCE_WEB_COLOR_NOSPLIT=2 IDO52_COPYPROP_SITE=lod:2:deny,lod:8:deny,lod:14:deny,lod:16:deny,lod:18:deny,lod:24:deny,lod:36:deny IDO52_AS1_TIE_REJECT=1376 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: remap_stack_slots --map '68:80' */
#include "types.h"

extern void srand(s32);
extern void line_set(s16, s16, s16, s16, s32, s32, s32);
extern void wipe_tile_disp(s32);

extern s32 frame_counter;
extern u32 step_counter;
extern u8 now_month;
extern s32 fade_flag;
extern s32 wipe_kind;
extern u8 fade_bri;

s32 wipe_disp(void)
{
    s16 i;
    s16 j;
    s16 w;
    s16 h;
    s16 x0;
    s16 x1;
    s16 y0;
    s16 y1;
    s16 y;
    s32 n;
    s32 x;

    srand(frame_counter);
    if (step_counter == 0) {
        wipe_kind = now_month % 7;
    }
    if (wipe_kind != 6) {
        fade_flag = 2;
    }
    fade_bri += 8;
    if (fade_bri >= 128) {
        fade_bri = 128;
    }
    i = 0;
    switch (wipe_kind) {
    case 1:
        for (i = step_counter * 5; i < 80; i++) {
            y = -41 - i;
            line_set(-160, y, 160, y, 0, 0, 13);
            y = i - 41;
            line_set(-160, y, 160, y, 0, 0, 13);
        }
        break;
    case 2:
        for (i = step_counter; i < 16; i++) {
            for (j = 0; j < 10; j++) {
                y = j * 16 + i - 120;
                line_set(-160, y, 160, y, 0, 0, 13);
            }
        }
        break;
    case 3:
        for (i = step_counter * 32; i < 480; i++) {
            if (i < 160) {
                line_set(i - 160, -120, -160, i - 120, 0, 0, 13);
            } else if (i < 320) {
                line_set(i - 160, -120, i - 320, 39, 0, 0, 13);
            } else {
                line_set(160, i - 440, i - 320, 39, 0, 0, 13);
            }
        }
        break;
    case 4:
        for (i = step_counter * 64; i < 480; i += 4) {
            if (i < 160) {
                line_set(i - 160, -120, -160, i - 120, 0, 0, 13);
            } else if (i < 320) {
                line_set(i - 160, -120, i - 320, 39, 0, 0, 13);
            } else {
                line_set(160, i - 440, i - 320, 39, 0, 0, 13);
            }
        }
        for (i = step_counter * 64; i < 480; i += 4) {
            if (i < 160) {
                line_set(i - 159, -120, -160, i - 119, 0, 0, 13);
            } else if (i < 320) {
                line_set(i - 159, -120, i - 319, 39, 0, 0, 13);
            } else {
                line_set(160, i - 439, i - 319, 39, 0, 0, 13);
            }
        }
        if (step_counter * 64 < 480) {
            j = 0;
        } else {
            j = step_counter * 64 - 480;
        }
        for (i = j; i < 480; i += 4) {
            if (i < 160) {
                line_set(i - 158, -120, -160, i - 118, 0, 0, 13);
            } else if (i < 320) {
                line_set(i - 158, -120, i - 318, 39, 0, 0, 13);
            } else {
                line_set(160, i - 438, i - 318, 39, 0, 0, 13);
            }
        }
        for (i = j; i < 480; i += 4) {
            if (i < 160) {
                line_set(i - 157, -120, -160, i - 117, 0, 0, 13);
            } else if (i < 320) {
                line_set(i - 157, -120, i - 317, 39, 0, 0, 13);
            } else {
                line_set(160, i - 437, i - 317, 39, 0, 0, 13);
            }
        }
        break;
    case 5:
        for (i = step_counter * 10; i < 160; i++) {
            n = 160 - i;
            x0 = n - 160;
            x1 = 159 - n;
            y0 = n / 2 - 120;
            line_set(x0, y0, x1, y0, 0, 0, 13);
            y1 = 39 - n / 2;
            line_set(x0, y1, x1, y1, 0, 0, 13);
            line_set(x0, y0, x0, y1, 0, 0, 13);
            line_set(160 - n, y0, x1, y1, 0, 0, 13);
        }
        break;
    case 6:
        if (step_counter < 16) {
            wipe_tile_disp(step_counter >> 1);
        } else {
            wipe_tile_disp(8);
        }
        break;
    default:
        for (i = 0; i < 15; i++) {
            n = step_counter;
            j = n * 2;
            n = i * 32;
            if (n < 160) {
                for (; j < 32; j++) {
                    x = n;
                    x += j;
                    line_set(x - 160, -120, -160, x - 120, 0, 0, 13);
                }
            } else if (n < 320) {
                for (; j < 32; j++) {
                    x = n;
                    x += j;
                    line_set(x - 160, -120, x - 320, 40, 0, 0, 13);
                }
            } else {
                for (; j < 32; j++) {
                    x = n;
                    x += j;
                    line_set(160, x - 440, x - 320, 40, 0, 0, 13);
                }
            }
        }
        break;
    }
    step_counter++;
    if (step_counter >= 16) {
        fade_flag = 2;
        return 1;
    }
    return 0;
}
