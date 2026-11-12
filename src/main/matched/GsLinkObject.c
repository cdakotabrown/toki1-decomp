/* Address: 0x8009D604 */
/* Size: 208 bytes (52 instructions) */
/* Region: Library (0x80086000+), Psy-Q GCC 2.6.0 */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 --fill-earliest */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0xbc --offset2 0xcc */
/* @PostProc: swap_instructions --offset1 0x0 --offset2 0x28 */
/* @PostProc: swap_instructions --offset1 0x4 --offset2 0x2c */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0x30 */
/* @PostProc: swap_instructions --offset1 0xc --offset2 0x34 */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x38 */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x38 */
/* @PostProc: swap_instructions --offset1 0x1c --offset2 0x38 */
/* @PostProc: swap_instructions --offset1 0x20 --offset2 0x3c */
/* @PostProc: swap_instructions --offset1 0x24 --offset2 0x38 */
/* @PostProc: swap_instructions --offset1 0x38 --offset2 0x3c */
/* @PostProc: swap_instructions --offset1 0x4c --offset2 0x58 */
/* @PostProc: swap_instructions --offset1 0x50 --offset2 0x54 */
/* @PostProc: swap_instructions --offset1 0x5c --offset2 0x68 */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x64 */
/* @PostProc: swap_instructions --offset1 0x6c --offset2 0x78 */
/* @PostProc: swap_instructions --offset1 0x70 --offset2 0x74 */
/* @PostProc: swap_instructions --offset1 0x7c --offset2 0x88 */
/* @PostProc: swap_instructions --offset1 0x80 --offset2 0x84 */
/* @PostProc: swap_instructions --offset1 0x8c --offset2 0x98 */
/* @PostProc: swap_instructions --offset1 0x90 --offset2 0x94 */
/* @PostProc: swap_instructions --offset1 0x9c --offset2 0xa8 */
/* @PostProc: swap_instructions --offset1 0xa0 --offset2 0xa4 */
/* @PostProc: swap_instructions --offset1 0xac --offset2 0xb8 */
/* @PostProc: swap_instructions --offset1 0xb0 --offset2 0xb4 */
/* @PostProc: swap_instructions --offset1 0xc0 --offset2 0xc4 */
#include "types.h"

typedef struct {
    u32 w[8];
} Rec8;

typedef struct {
    u8 e[28];
} Ent28;

typedef struct {
    u32 flg;
    Rec8 m1;
    Rec8 m2;
    s32 rx, ry, rz;
    s16 sx, sy, sz;
    s16 pad;
    s32 tx, ty, tz;
    void *p100;
    void *p104;
    Ent28 *link;
} Obj112;

extern Rec8 GsIDMATRIX;

void GsLinkObject(Ent28 *tbl, Obj112 *o, s32 idx) {
    o->tx = o->ty = o->tz = 0;
    o->rx = o->ry = o->rz = 0;
    o->sx = o->sy = o->sz = 4096;
    o->link = &tbl[idx];
    o->p100 = 0;
    o->p104 = 0;
    o->m1 = GsIDMATRIX;
    o->m2 = GsIDMATRIX;
}
