/* Address: 0x8004A510 */
/* Size: 628 bytes (157 instructions) */
/* Region: Game (IDO 5.2) */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u8 b0;
    u8 b1;
    u16 h2;
    u16 h4;
    u16 h6;
} BG_CELL;

typedef struct {
    u8 r;
    s8 s;
    s16 cols;
    s16 rows;
    s16 pad;
    BG_CELL *cells;
    u16 *idx;
} BG_MAP;

typedef struct {
    u32 flags;
    u16 h4;
    u16 h6;
    s16 w;
    s16 h;
    s16 x;
    s16 y;
    u8 r, g, b, pad;
    BG_MAP *map;
    s16 h24;
    s16 h26;
    s16 sx;
    s16 sy;
    u32 w32;
} BG_WORK;

extern BG_WORK Bg[];
extern BG_MAP Map[];
extern BG_CELL Cell[][64];
extern u16 C_index[][64];
extern u8 bg_pri[];
extern char bg_set_str0[];

extern void load_tpage_buf_lock();
extern s32 GetClut();
extern void printf();

void bg_set(src, slot, unused, cols, rows, arg5, arg6, arg7, arg8, base)
    s32 *src;
    s32 slot;
    s32 unused;
    s32 cols;
    s32 rows;
    s32 arg5;
    s32 arg6;
    s32 arg7;
    s32 arg8;
    s32 base;
{
    s32 pad[2];
    s32 row;
    s32 col;

    Bg[slot].flags = 0x1000000;
    Bg[slot].h4 = arg6;
    Bg[slot].h6 = arg7;
    Bg[slot].w = cols << 7;
    Bg[slot].h = rows * 120;
    Bg[slot].sx = 4096;
    Bg[slot].sy = 4096;
    Bg[slot].map = &Map[slot];
    Bg[slot].x = 0;
    Bg[slot].y = 0;
    Bg[slot].r = 128;
    Bg[slot].g = 128;
    Bg[slot].b = 128;
    Bg[slot].h24 = 0;
    Bg[slot].h26 = 0;
    Bg[slot].w32 = 0;
    Map[slot].r = 128;
    Map[slot].s = 120;
    Map[slot].cols = cols;
    Map[slot].cells = Cell[slot];
    Map[slot].idx = C_index[slot];
    Map[slot].rows = rows;
    for (row = 0; row < rows; row++) {
        for (col = 0; col < cols; col++) {
            load_tpage_buf_lock(base + col + row * 32, src[row * cols + col], 1);
        }
    }
    for (row = 0; row < rows; row++) {
        for (col = 0; col < cols; col++) {
            C_index[slot][row * cols + col] = row * cols + col;
            Cell[slot][row * cols + col].b0 = 0;
            Cell[slot][row * cols + col].b1 = row * 120;
            Cell[slot][row * cols + col].h6 = base + col;
            Cell[slot][row * cols + col].h2 = GetClut(0, arg5);
            Cell[slot][row * cols + col].h4 = 0;
            if (row * cols + col >= 65) {
                printf(bg_set_str0);
            }
        }
    }
    bg_pri[slot] = arg8;
}
