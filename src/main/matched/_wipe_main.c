/* Address: 0x8007D9F8 (true start 0x8007D9F0: the fade_bri read is */
/* Size: 2788 bytes (697 instructions) */
/* Region: Game (IDO 5.2) */
/* @SymbolStart: 0x8007D9F0 */
/* @SymbolSize: 2788 */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u32 step_counter;
extern u8 now_month;
extern s32 wipe_kind;
extern s32 fade_flag;
extern u8 fade_bri;

extern void line_set(s16, s16, s16, s16, u32, u32, u32);
extern void wipe_tile_disp();

s32 _wipe_main()
{
    s16 i;
    s16 n;
    s16 j;
    s16 y;
    s16 y2;
    s16 x2;
    s32 v;

    fade_bri -= 8;
    if (fade_bri >= 129) {
        fade_bri = 0;
    }
    if (step_counter == 0) {
        wipe_kind = now_month % 7;
    }
    if (wipe_kind == 6) {
        fade_flag = 0;
    }
    switch (wipe_kind) {
    case 1:
        n = (s16)(*(s32 *)&step_counter * 5);
        if (n >= 81) {
            n = 80;
        }
        for (i = 0; i < n; i++) {
            y = i - 120;
            line_set(-160, y, 160, y, 0, 0, 6);
            y2 = 39 - i;
            line_set(-160, y2, 160, y2, 0, 0, 6);
        }
        break;
    case 2:
        n = *(s16 *)&step_counter;
        if (n >= 17) {
            n = 16;
        }
        for (i = 0; i < n; i++) {
            for (j = 0; j < 10; j++) {
                y = (j << 4) - i - 105;
                line_set(-160, y, 160, y, 0, 0, 6);
            }
        }
        break;
    case 3:
        n = (s16)(step_counter << 5);
        j = 479;
        if (n >= 481) {
            n = 480;
        }
        for (i = 0; i < n; i++) {
            v = j - i;
            if (i >= 321) {
                line_set(v - 160, -120, -160, v - 120, 0, 0, 13);
            } else if (i >= 161) {
                line_set(v - 160, -120, v - 320, 39, 0, 0, 13);
            } else {
                line_set(160, v - 440, v - 320, 39, 0, 0, 13);
            }
        }
        break;
    case 4:
        n = (s16)(step_counter << 6);
        j = 479;
        if (n >= 481) {
            n = 480;
        }
        for (i = 0; i < n; i += 4) {
            v = j - i;
            if (i >= 321) {
                line_set(v - 160, -120, -160, v - 120, 0, 0, 13);
            } else if (i >= 161) {
                line_set(v - 160, -120, v - 320, 39, 0, 0, 13);
            } else {
                line_set(160, v - 440, v - 320, 39, 0, 0, 13);
            }
        }
        for (i = 0, j = 479; i < n; i += 4) {
            v = j - i;
            if (i >= 321) {
                line_set(v - 159, -120, -160, v - 119, 0, 0, 13);
            } else if (i >= 161) {
                line_set(v - 159, -120, v - 319, 39, 0, 0, 13);
            } else {
                line_set(160, v - 439, v - 319, 39, 0, 0, 13);
            }
        }
        n = (s16)(step_counter * 64 - 480);
        if (n >= 481) {
            n = 480;
        }
        for (i = 0; i < n; i += 4) {
            v = j - i;
            if (i >= 321) {
                line_set(v - 158, -120, -160, v - 118, 0, 0, 13);
            } else if (i >= 161) {
                line_set(v - 158, -120, v - 318, 39, 0, 0, 13);
            } else {
                line_set(160, v - 438, v - 318, 39, 0, 0, 13);
            }
        }
        for (i = 0; i < n; i += 4) {
            v = j - i;
            if (i >= 321) {
                line_set(v - 157, -120, -160, v - 117, 0, 0, 13);
            } else if (i >= 161) {
                line_set(v - 157, -120, v - 317, 39, 0, 0, 13);
            } else {
                line_set(160, v - 437, v - 317, 39, 0, 0, 13);
            }
        }
        break;
    case 5:
        n = (s16)(step_counter * 10);
        if (n >= 161) {
            n = 160;
        }
        for (i = 0; i < n; i++) {
            j = i - 160;
            x2 = 159 - i;
            v = i / 2;
            y = v - 120;
            line_set(j, y, x2, y, 0, 0, 13);
            y2 = 39 - v;
            line_set(j, y2, x2, y2, 0, 0, 13);
            line_set(j, y, j, y2, 0, 0, 13);
            line_set(160 - i, y, x2, y2, 0, 0, 13);
        }
        break;
    case 6:
        if (step_counter < 16) {
            wipe_tile_disp(8 - (step_counter >> 1));
        } else {
            wipe_tile_disp(0);
        }
        break;
    default:
        n = (s16)(step_counter << 1);
        if (n >= 33) {
            n = 32;
        }
        i = 0;
        do {
            v = i << 5;
            j = 0;
            if (v < 160) {
                for (v += 31; j < n; j++) {
                    line_set(v - j - 160, -120, -160, v - j - 120, 0, 0, 13);
                }
            } else if (v < 320) {
                for (v += 31; j < n; j++) {
                    line_set(v - j - 160, -120, v - j - 320, 40, 0, 0, 13);
                }
            } else {
                for (v += 31; j < n; j++) {
                    line_set(160, v - j - 440, v - j - 320, 40, 0, 0, 13);
                }
            }
            i++;
        } while (i < 15);
        break;
    }
    step_counter++;
    if (step_counter >= 18) {
        fade_flag = 0;
        return 1;
    }
    return 0;
}
