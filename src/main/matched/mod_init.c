/* Address: 0x8004E884 */
/* Size: 172 bytes (43 instructions) -- the recorded 168 cuts off the jr-ra delay-slot store */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @SymbolStart: 0x8004E884 */
/* @SymbolSize: 172 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    s32  f0;
    u32 *f4;
    u8   f8;
    u8   f9;
    u16  fA;
    u8   fC;
    u8   fD;
    u8   fE;
    u8   fF;
    u16  f10;
    u16  f12;
} Rec20;

extern Rec20 mod_w[];

void mod_init(s32 idx, u32 *src, s32 arg2) {
    Rec20 *p = &mod_w[idx];

    p->f0 = arg2;
    p->f4 = src + 4;
    p->f8 = (src[0] & 0xFF000000) >> 24;
    p->fA = (src[0] & 0xFFFF) >> 24;
    p->fC = (src[1] & 0xFF000000) >> 24;
    p->fD = (src[1] & 0x00FF0000) >> 16;
    p->fE = (src[1] & 0xFF00) >> 8;
    p->fF = src[1] & 0xFF;
    p->f10 = ((u8 *)src)[8];
    p->f12 = ((u8 *)src)[12];
}
