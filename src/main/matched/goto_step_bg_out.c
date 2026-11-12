/* Function: goto_step_bg_out (o.bin symbol; the 2-word head at 0x80057A78 that the */
/* Address: 0x80057A80 */
/* Size: 476 bytes (119 instructions) + 8-byte head = 484 */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolStart: 0x80057A78 */
/* @SymbolSize: 484 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern u8 fade_bri;
extern s32 goto_step_bg_out_tbl0;
extern void line_set(s16, s16, s16, s16, s32, s32, s32);

s32 goto_step_bg_out(void) {
    s16 row;
    s16 col;
    s16 limit;
    s32 x;
    s32 top;
    s32 v;
    u8 idx;

    fade_bri = fade_bri - 8;
    idx = fade_bri;
    if (idx >= 129U) {
        fade_bri = 0;
    }

    v = ++goto_step_bg_out_tbl0 * 2;
    limit = (s16)v;
    if ((s16)v >= 33) {
        limit = 32;
    }

    for (row = 0; row < 15; row = (s16)(row + 1)) {
        x = row * 32;
        col = 0;
        if (x < 160) {
            if (limit > 0) {
                top = x + 31;
                do {
                    v = top - col;
                    line_set((s16)(v - 160), -120, -160, (s16)(v - 120), 0, 0, 13);
                    col = (s16)(col + 1);
                } while (col < limit);
            }
        } else if (x < 320) {
            if (limit > 0) {
                top = x + 31;
                do {
                    v = top - col;
                    line_set((s16)(v - 160), -120, (s16)(v - 320), 40, 0, 0, 13);
                    col = (s16)(col + 1);
                } while (col < limit);
            }
        } else {
            if (limit > 0) {
                top = x + 31;
                do {
                    v = top - col;
                    line_set(160, (s16)(v - 440), (s16)(v - 320), 40, 0, 0, 13);
                    col = (s16)(col + 1);
                } while (col < limit);
            }
        }
    }
}
