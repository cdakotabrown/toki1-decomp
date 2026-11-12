/* @SymbolStart: 0x801326C0 */
/* @SymbolSize: 516 */
/* @CompileFlags: -cckr */
/* @PostProc: swap_instructions --offset1 0x94 --offset2 0x98 */
#include "types.h"

extern s32 screen_w[];
extern u8  screen_bri;
extern s32 frame_counter;
typedef struct {
    s16 x;
    s16 y;
    u8  r;
    u8  g;
    u8  b;
} Scroll;
extern Scroll bg0_scrollx;
extern s32 option_bg_flag;
extern s32 option_bg_pal_no;
extern s32 option_bg_cnt;

#define PAL ((s32 *)0x801CBEF0)

option_bg_main()
{
    s32 i;
    s32 *p;
    s32 cnt;

    if (option_bg_flag == 0) {
        return;
    }
    bg0_scrollx.r = screen_bri;
    bg0_scrollx.g = screen_bri;
    bg0_scrollx.b = screen_bri;
    if (frame_counter & 1) {
        bg0_scrollx.x++;
        bg0_scrollx.y--;
    }
    option_bg_cnt++;
    if (option_bg_cnt >= 257) {
        p = screen_w;
        for (i = 0; i < 128; i++) {
            p[i + 1423] = PAL[option_bg_pal_no % 4 * 128 + i];
        }
        p = screen_w;
        for (i = 0; i <= option_bg_cnt - 256; i++) {
            p[i + 1439] = PAL[(option_bg_pal_no + 1) % 4 * 128 + i + 16];
            p[i + 1479] = PAL[(option_bg_pal_no + 1) % 4 * 128 + i + 56];
            if (i + 96 < 128) {
                p[i + 1519] = PAL[(option_bg_pal_no + 1) % 4 * 128 + i + 96];
            }
        }
        if (option_bg_cnt >= 297) {
            option_bg_pal_no++;
            option_bg_cnt = 0;
        }
        palette_upload_enqueue(&screen_w[1423], 26, 1, 1, 0);
    } else {
        return;
    }
}
