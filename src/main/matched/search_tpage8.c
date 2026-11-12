/* Function: search_tpage8 -- sprite-slot lookup / allocator (shift 8) */
/* Address: 0x80048DD0 (prologue at 80048DD8 -- see HEAD ABSORPTION) */
/* Size: 616 bytes (154 instructions) */
/* Region: Game (IDO 5.2) */
/* @SymbolStart: 0x80048DD0 */
/* @SymbolSize: 616 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

typedef struct {
    u8 pad[52];
    s32 *keys;
    u8 pad2[12];
} Chr68;

extern void image_upload_queue_add_flags(s32, u8, u8, s32, s32, s32, s32);
typedef struct {
    u8 pad[0x1128];
    s32 state[64];
    s32 key[64];
} SprSlots;

extern SprSlots screen_w;
extern Chr68 class_w[];

s32 search_tpage8(s16 id, s32 chr, u8 arg2)
{
    Chr68 *p;
    s32 i;
    s32 j;

    if (id == 0x7FFF) {
        for (i = 0; i < 64; i++) {
            if (screen_w.key[i] == -1) {
                screen_w.state[i] = 0x7FFF;
            } else {
                screen_w.state[i] = 0x7FFE;
            }
        }
        return 0;
    }
    p = (Chr68 *)((s32)class_w + chr * 68);
    for (i = 15; i >= 5; i--) {
        for (j = 3; j >= 0; j--) {
            if (screen_w.state[i + j * 16] == 0x7FFE && screen_w.key[i + j * 16] == p->keys[(s16)(id >> 8)]) {
                return (u16)(i + j * 16);
            }
        }
    }
    for (i = 0; i < 64; i++) {
        if (screen_w.state[i] == p->keys[(s16)(id >> 8)]) {
            return (u16)i;
        }
    }
    for (i = 0; i < 64; i++) {
        if (screen_w.state[i] == 0x7FFF) {
            screen_w.state[i] = p->keys[(s16)(id >> 8)];
            if (i < 32) {
                image_upload_queue_add_flags(p->keys[(s16)(id >> 8)], i, arg2, 0, 0, 128, 128);
            } else {
                image_upload_queue_add_flags(p->keys[(s16)(id >> 8)], i, arg2, 0, 128, 128, 128);
            }
            return (u16)i;
        }
        if (i == 63) {
            return 63;
        }
    }
}
