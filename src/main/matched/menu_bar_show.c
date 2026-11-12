/* Address: 0x80050F74 */
/* Size: 208 bytes (52 instructions) */
/* Region: Game (IDO 5.2) */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    s16 x;
    s16 y;
    u8 w;
    u8 h;
    u8 hit;
    u8 pad;
} Row;

typedef struct {
    u8 pad0[3984];
    Row row[16][2];
    u8 cnt[3];
    s8 sel[2];
    u8 chosen[2];
} Save;

extern Save screen_w;
extern u32 menu_bar_rgb[];

void menu_bar_show(slot)
    s32 slot;
{
    s32 idx;
    u32 tp;
    u32 abr;
    u32 semi;

    idx = screen_w.sel[slot];
    tp = (menu_bar_rgb[slot] >> 24) & 0xF;
    abr = (menu_bar_rgb[slot] >> 28) & 7;
    semi = (menu_bar_rgb[slot] >> 31) << 7;
    if (idx >= 0 && idx < screen_w.cnt[slot]) {
        _sprite_set_box_shade(screen_w.row[idx][slot].x, screen_w.row[idx][slot].y,
                      screen_w.row[idx][slot].w, screen_w.row[idx][slot].h,
                      tp, menu_bar_rgb[slot] & 0xFFFFFF, semi | abr);
        dtd_on_tpage(0, 0, tp, 0, 1);
    }
}
