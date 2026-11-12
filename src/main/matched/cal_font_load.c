/* Address: 0x80068A14 (true start 0x800689FC: the month/day index */
/* Size: 684 bytes (171 instructions) */
/* Region: Game (IDO 5.2) */
/* @SymbolStart: 0x800689FC */
/* @SymbolSize: 684 */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern s32 **cal_font_load_tbl0;
typedef struct {
    u8 pad0[68];
    u8 prev[64];
    u8 cnt;
    u8 a[31];
    u8 cnt2;
    u8 b[31];
} Save;

extern Save screen_w;

extern void load_image_rect();
extern void memcpy();

void cal_font_load(month, day, flag)
    u8 month;
    u8 day;
    s32 flag;
{
    s32 idx;
    s32 n;
    s32 i;
    Save *sv;
    s32 *data;
    s32 m;

    idx = month * 12 + day - 1144;
    if (idx >= 35) {
        return;
    }
    load_image_rect(928, 256, 32, 128, cal_font_load_tbl0[idx]);
    load_image_rect(928, 384, 32, 128, cal_font_load_tbl0[idx + 1]);
    if (flag == 0) {
        return;
    }
    sv = &screen_w;
    memcpy(screen_w.prev, &screen_w.cnt, 64);
    data = cal_font_load_tbl0[idx + 1] + 2048;
    n = data[0];
    m = data[1];
    screen_w.cnt = n;
    screen_w.cnt2 = m;
    for (i = 0; i < n; i++) {
        sv->a[i] = cal_font_load_tbl0[idx + 1][2050 + i];
        sv->b[i] = cal_font_load_tbl0[idx + 1][2050 + n + i];
    }
}
