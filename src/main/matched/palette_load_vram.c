/* Function: palette_load_vram */
/* Address: 0x80043D80 (the palette_queue_n read is hoisted above the prologue) */
/* Size: 2024 bytes (506 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolStart: 0x80043D80 */
/* @SymbolSize: 2024 */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u32 *clut;
    u8 pal;
    u8 pad;
    u8 mode;
    u8 row;
} ClutEnt;

typedef struct {
    u8 pad[4904];
    ClutEnt ent[32];
    u8 count;
} Save;

extern u8 palette_queue_n;
extern Save screen_w;

extern void load_image_rect(u16, u16, u16, s32, u32 *);

void palette_load_vram(arg0)
s32 arg0;
{
    Save *sv;
    u16 x;
    u16 y;
    u16 w;
    s32 i;
    s32 j;
    s32 start;
    s32 unused[2];

    sv = &screen_w;
    i = 0;
    if (palette_queue_n != 0) {
        do {
            y = (sv->ent[i].pal & 0x1F) + 480;
            start = sv->ent[i].row << 7;
            if (sv->ent[i].pal < 32) {
                w = 256;
                x = 0;
                switch (sv->ent[i].mode) {
                case 0:
                    for (j = start; j < start + 128; j++) {
                        if (sv->ent[i].clut[j] & 0x80000000) {
                            sv->ent[i].clut[j] &= 0x7FFF7FFF;
                        }
                        if (sv->ent[i].clut[j] & 0x8000) {
                            sv->ent[i].clut[j] &= 0x7FFF7FFF;
                        }
                    }
                    break;
                case 1:
                default:
                    break;
                case 2:
                    for (j = start; j < start + 120; j++) {
                        if (sv->ent[i].clut[j] & 0x80000000) {
                            sv->ent[i].clut[j] &= 0x7FFF7FFF;
                        }
                        if (sv->ent[i].clut[j] & 0x8000) {
                            sv->ent[i].clut[j] &= 0x7FFF7FFF;
                        }
                    }
                    for (j = start + 120; j < start + 128; j++) {
                        if (sv->ent[i].clut[j] & 0x7FFF0000) {
                            sv->ent[i].clut[j] |= 0x80000000;
                        }
                        if (sv->ent[i].clut[j] & 0x7FFF) {
                            sv->ent[i].clut[j] |= 0x8000;
                        }
                    }
                    break;
                case 3:
                    for (j = start; j < start + 128; j++) {
                        if (sv->ent[i].clut[j] & 0x7FFF0000) {
                            sv->ent[i].clut[j] |= 0x80000000;
                        }
                        if (sv->ent[i].clut[j] & 0x7FFF) {
                            sv->ent[i].clut[j] |= 0x8000;
                        }
                    }
                    break;
                case 4:
                    for (j = start; j < start + 112; j++) {
                        if (sv->ent[i].clut[j] & 0x80000000) {
                            sv->ent[i].clut[j] &= 0x7FFF7FFF;
                        }
                        if (sv->ent[i].clut[j] & 0x8000) {
                            sv->ent[i].clut[j] &= 0x7FFF7FFF;
                        }
                    }
                    for (j = start + 112; j < start + 128; j++) {
                        if (sv->ent[i].clut[j] & 0x7FFF0000) {
                            sv->ent[i].clut[j] |= 0x80000000;
                        }
                        if (sv->ent[i].clut[j] & 0x7FFF) {
                            sv->ent[i].clut[j] |= 0x8000;
                        }
                    }
                    break;
                }
            } else {
                w = 32;
                if (sv->ent[i].pal < 64) {
                    x = 256;
                } else {
                    x = 288;
                }
            }
            load_image_rect(x, y, w, 1, &sv->ent[i].clut[start]);
            i++;
        } while (i < sv->count);
    }
    palette_queue_n = 0;
}
