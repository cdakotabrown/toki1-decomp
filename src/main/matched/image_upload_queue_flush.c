/* Address: 0x80043C90 */
/* Size: 240 bytes (60 instructions) */
/* Region: Game (0x80041000-0x80085FFF) - 64-byte frame */
/* Compiler: IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

typedef struct {
    s32 extra;
    u8 flags;
    u8 shift;
    u16 pad;
    u16 x;
    u16 y;
    u16 w;
    u16 h;
} Entry;

typedef struct {
    u8 pad0[5161];
    u8 count;
    u8 pad1[2];
    Entry e[1];
} Table;

extern Table screen_w;
extern u8 image_queue_n;

extern void load_image_rect(u16 x, u16 y, u16 w, u16 h, s32 extra);

void image_upload_queue_flush(s32 arg0) {
    Table *base;
    u32 i;

    base = &screen_w;
    i = 0;
    if (image_queue_n != 0) {
        do {
            if (base->e[i].flags == 0) {
                load_image_rect(base->e[i].x, base->e[i].y, base->e[i].w,
                              base->e[i].h, base->e[i].extra);
            } else {
                u16 x;
                u16 y;
                u16 w;

                x = base->e[i].x + ((base->e[i].flags & 0xF) << 6);
                y = base->e[i].y + ((base->e[i].flags & 0x10) << 4);
                w = (u32)base->e[i].w >> base->e[i].shift;
                load_image_rect(x, y, w, base->e[i].h, base->e[i].extra);
            }
            i++;
        } while (i < base->count);
    }

    image_queue_n = 0;
}
