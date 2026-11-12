/* Address: 0x800A34FC */
/* Size: 68 bytes (17 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @SymbolStart: 0x800A34FC */
/* @SymbolSize: 60 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    s16 h0;
    s16 h2;
    s16 h4;
    s16 h6;
    s16 h8;
    s16 h10;
    s16 h12;
    s16 h14;
    u8  b16;
    u8  b17;
    u8  b18;
    u8  b19;
} Prim20;

Prim20 *SetDefDispEnv(Prim20 *p, s32 a, s32 b, s32 c, s32 d) {
    p->h0 = a;
    p->h2 = b;
    p->h4 = c;
    p->h6 = d;
    p->h14 = 0;
    p->h12 = 0;
    p->h10 = 0;
    p->h8 = 0;
    p->b17 = 0;
    p->b16 = 0;
    p->b18 = 0;
    p->b19 = 0;
    return p;
}
