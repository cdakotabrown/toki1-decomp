/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=54:a2,71:a3,84:t0,119:a1 IDO52_CONST_NOINREG_VALUES=2149154816 */
/* @SymbolSize: 920 */
#include "types.h"

typedef struct {
    s16 x, y, w, h;
} Rect;

typedef struct {
    u16 px;
    u16 pad;
    u32 r;
    u32 g;
    u32 b;
} Pix;

extern void StoreImage();
extern void LoadImage();
extern void DrawSync();
extern void etc_col_split();

void etc_vram_blur(wa, wb)
u32 wa;
u32 wb;
{
    Rect rect;
    s32 p0, p1, p2;
    u16 *src;
    u16 *dst;
    s32 y;
    Pix buf[3][274];
    s32 i;
    u32 r;
    u32 g;
    u32 b;
    u32 nr;
    u32 ng;
    u32 nb;

    rect.x = 343;
    rect.y = 275;
    rect.w = 274;
    rect.h = 160;
    StoreImage(&rect, 0x80198000);
    DrawSync(0);
    src = (u16 *)0x80198000;
    dst = src;
    for (y = 0; y < 160; y++) {
        if (y == 0) {
            for (i = 0; i < 274; i++) {
                buf[2][i].px = *src;
                etc_col_split(&buf[2][i]);
                buf[0][i].px = *src;
                etc_col_split(&buf[0][i]);
                src++;
            }
        }
        for (i = 0; i < 274; i++) {
            if (y == 159) {
                buf[(y + 1) % 3][i].px = buf[y % 3][i].px;
                etc_col_split(&buf[(y + 1) % 3][i]);
            } else {
                buf[(y + 1) % 3][i].px = *src;
                etc_col_split(&buf[(y + 1) % 3][i]);
                src++;
            }
        }
        dst++;
        for (i = 1; i < 273; i++) {
            r = buf[(y + 2) % 3][i].r + buf[(y + 1) % 3][i].r + buf[y % 3][i - 1].r + buf[y % 3][i + 1].r;
            r >>= 2;
            g = buf[(y + 2) % 3][i].g + buf[(y + 1) % 3][i].g + buf[y % 3][i - 1].g + buf[y % 3][i + 1].g;
            g >>= 2;
            b = buf[(y + 2) % 3][i].b + buf[(y + 1) % 3][i].b + buf[y % 3][i - 1].b + buf[y % 3][i + 1].b;
            b >>= 2;
            nr = buf[y % 3][i].r * wa / 100 + r * wb / 100;
            ng = buf[y % 3][i].g * wa / 100 + g * wb / 100;
            nb = buf[y % 3][i].b * wa / 100 + b * wb / 100;
            *dst = (nr & 0x1F) | (ng & 0x3E0) | (nb & 0x7C00);
            *dst |= 0x8000;
            dst++;
        }
        dst++;
    }
    LoadImage(&rect, 0x80198000);
    DrawSync(0);
}
