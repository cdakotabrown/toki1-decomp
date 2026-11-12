/* Address: 0x8004D728 */
/* Size: 752 bytes (188 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: remap_stack_slots --map 116:120,104:116 */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @CompileEnv: IDO52_COPYPROP_VAR=-302,-304,-296 IDO52_COPYPROP_SITE=lod:7:deny,lod:9:deny IDO52_FORCE_WEB_COLOR=76:t0,87:v1 IDO52_UGEN_TEMP_REG=38:t1,39:t3,41:t7,42:t2,43:t5,47:t9,48:t8 IDO52_AS1_TIE_TAKE=223,235,241 IDO52_AS1_TIE_REJECT=229 */
#include "types.h"

typedef struct {
    u8 **ptr0;
    u32 *ptr4;
    u8 type;
    u8 pad[3];
    u8 tw;
    u8 th;
    u8 tileSz;
    u8 pad2;
    u32 data;
} TileEntry;

extern TileEntry mod_w[];

extern void get_fnt(u8 *, u8 *, s32, s32, s32, s32);
extern void load_image_rect(u16, u16, u16, u16, u8 *);

void mod_trans_vram(s32 idx, u32 tileId, s32 destX, s32 destY, s32 arg4) {
    s32 pad1;
    u32 hi;
    u32 row;
    TileEntry *entry;
    u32 col;
    u32 *srcPtr;
    u32 word;
    u32 m3;
    u8 buf[248];
    u8 cs;
    u32 tw;
    u8 th;
    s32 csVal;
    s32 tileSize;
    u16 w, h, y;

    entry = &mod_w[idx];

    if (entry->tileSz == 8) {
        tileSize = 8;
    } else {
        tileSize = 16;
    }

    switch (entry->type) {
    case 0: cs = 1; break;
    case 1: cs = 2; break;
    case 2: cs = 2; break;
    case 3: cs = 1; break;
    case 4: cs = 1; break;
    case 5: cs = 1; break;
    default: cs = 1; break;
    }

    th = entry->th;
    tw = entry->tw;
    srcPtr = th * tileId * tw + entry->ptr4;

    row = 0;
    if (th != 0) {
      do {
        col = 0;
        tw = entry->tw;
        if (tw != 0) {
          csVal = cs;
          w = (u32)(tileSize / 2) / cs;
          h = tileSize;
          y = ((destX & 0x10) << 4) + ((destX & 0x20) << 2) + arg4 + row * tileSize;
          do {
            word = *srcPtr;
            m3 = word & 0xFF0000;
            m3 >>= 16;
            hi = word & 0xFF000000;
            get_fnt(buf, entry->ptr0[hi >> 24], m3, csVal, tileSize, word & 0xFF);
            load_image_rect(((destX & 0xF) << 6) + (u32)(destY / 2) / cs + (u32)((s32)(col * tileSize) / 2) / cs, y, w, h, buf);
            srcPtr++;
            col++;
            tw = entry->tw;
          } while (col < tw);
        }
        row++;
        th = entry->th;
      } while (row < th);
    }
}
