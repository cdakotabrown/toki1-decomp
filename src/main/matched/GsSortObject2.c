/* Function: GsSortObject2 */
/* Address: 0x8009D388 */
/* Size: 636 bytes (159 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 --no-cross-jumping --fill-earliest --fill-load-delay */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0xC */
/* @PostProc: swap_instructions --offset1 0xC --offset2 0x18 */
/* @PostProc: swap_instructions --offset1 0x10 --offset2 0x1C */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x20 */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x24 */
/* @PostProc: swap_instructions --offset1 0x1C --offset2 0x28 */
/* @PostProc: swap_instructions --offset1 0x20 --offset2 0x24 */
/* @PostProc: swap_instructions --offset1 0x24 --offset2 0x2C */
/* @PostProc: swap_instructions --offset1 0x28 --offset2 0x30 */
/* @PostProc: swap_instructions --offset1 0x2C --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x30 --offset2 0x34 */
/* @PostProc: swap_instructions --offset1 0x34 --offset2 0x48 */
/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x50 */
/* @PostProc: swap_instructions --offset1 0x44 --offset2 0x4C */
/* @PostProc: swap_instructions --offset1 0x48 --offset2 0x58 */
/* @PostProc: swap_instructions --offset1 0x4C --offset2 0x5C */
/* @PostProc: swap_instructions --offset1 0x58 --offset2 0x60 */
/* @PostProc: swap_instructions --offset1 0x5C --offset2 0x64 */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x68 */
/* @PostProc: swap_instructions --offset1 0x64 --offset2 0x6C */
/* @PostProc: swap_instructions --offset1 0x68 --offset2 0x74 */
/* @PostProc: swap_instructions --offset1 0x6C --offset2 0x78 */
/* @PostProc: swap_instructions --offset1 0x70 --offset2 0x7C */
/* @PostProc: swap_instructions --offset1 0x74 --offset2 0x7C */
/* @PostProc: swap_instructions --offset1 0x78 --offset2 0x80 */
/* @PostProc: swap_instructions --offset1 0x7C --offset2 0x84 */
/* @PostProc: swap_instructions --offset1 0x80 --offset2 0x88 */
/* @PostProc: swap_instructions --offset1 0x84 --offset2 0x8C */
/* @PostProc: swap_instructions --offset1 0x88 --offset2 0x90 */
/* @PostProc: swap_instructions --offset1 0x8C --offset2 0x94 */
/* @PostProc: swap_instructions --offset1 0x90 --offset2 0x9C */
/* @PostProc: swap_instructions --offset1 0x94 --offset2 0xA0 */
/* @PostProc: swap_instructions --offset1 0x98 --offset2 0xA4 */
/* @PostProc: swap_instructions --offset1 0x9C --offset2 0xA4 */
/* @PostProc: swap_instructions --offset1 0xA0 --offset2 0xA8 */
/* @PostProc: swap_instructions --offset1 0xA4 --offset2 0xAC */
/* @PostProc: swap_instructions --offset1 0xA8 --offset2 0xB0 */
/* @PostProc: swap_instructions --offset1 0xAC --offset2 0xB4 */
/* @PostProc: swap_instructions --offset1 0xB0 --offset2 0xB8 */
/* @PostProc: swap_instructions --offset1 0xB4 --offset2 0xBC */
/* @PostProc: swap_instructions --offset1 0xB8 --offset2 0xC4 */
/* @PostProc: swap_instructions --offset1 0xBC --offset2 0xC8 */
/* @PostProc: swap_instructions --offset1 0xC0 --offset2 0xCC */
/* @PostProc: swap_instructions --offset1 0xC4 --offset2 0xCC */
/* @PostProc: swap_instructions --offset1 0xC8 --offset2 0xD0 */
/* @PostProc: swap_instructions --offset1 0xCC --offset2 0xD4 */
/* @PostProc: swap_instructions --offset1 0xD0 --offset2 0xD8 */
/* @PostProc: swap_instructions --offset1 0xDC --offset2 0xE0 */
/* @PostProc: swap_instructions --offset1 0x200 --offset2 0x204 */
/* @PostProc: swap_instructions --offset1 0x204 --offset2 0x208 */
/* @PostProc: swap_instructions --offset1 0x208 --offset2 0x210 */
/* @PostProc: swap_instructions --offset1 0x20C --offset2 0x214 */
/* @PostProc: swap_instructions --offset1 0x210 --offset2 0x218 */
/* @PostProc: swap_instructions --offset1 0x214 --offset2 0x21C */
/* @PostProc: swap_instructions --offset1 0x218 --offset2 0x220 */
/* @PostProc: swap_instructions --offset1 0x21C --offset2 0x22C */
/* @PostProc: swap_instructions --offset1 0x220 --offset2 0x22C */
/* @PostProc: swap_instructions --offset1 0x224 --offset2 0x234 */
/* @PostProc: swap_instructions --offset1 0x228 --offset2 0x234 */
/* @PostProc: swap_instructions --offset1 0x22C --offset2 0x238 */
/* @PostProc: swap_instructions --offset1 0x230 --offset2 0x23C */
/* @PostProc: swap_instructions --offset1 0x234 --offset2 0x240 */
/* @PostProc: swap_instructions --offset1 0x238 --offset2 0x244 */
/* @PostProc: swap_instructions --offset1 0x23C --offset2 0x248 */
/* @PostProc: swap_instructions --offset1 0x240 --offset2 0x24C */
/* @PostProc: swap_instructions --offset1 0x244 --offset2 0x250 */
/* @PostProc: swap_instructions --offset1 0x248 --offset2 0x250 */
/* (2) DELETED `@PostProc: scheduler_normalizer`.  On a body that is 8 words */
#include "types.h"

typedef s32 (*DispatchFunc)(s32, s32, s32, s32);

typedef struct {
    u32 flags;
    u32 unk04;
    s32 *seq;
} SpuVmDesc;

extern s32 libgs_obj_attr_bits0to2;
extern s32 GsLMODE;
extern s32 GsLIGNR;
extern s32 GsLIOFF;
extern s32 libgs_obj_attr_bit7;
extern s32 libgs_obj_attr_bit8;
extern s32 GsNDIV;
extern s32 GsTRATE;
extern s32 GsTON;
extern s32 GsDISPON;
extern s32 GsLIGHT_FUNC;
extern s32 GsLIGHT_MODE;
extern s32 GsOUT_PACKET_P;

extern s32 Gslight_normal();
extern s32 Gslight_normal_fog();
extern s32 Gslight_mate();
extern s32 Gslight_mate_fog();

void GsSortObject2(SpuVmDesc *arg0, s32 *arg1, s32 arg2) {
    s32 *seq;
    s32 *base;
    u32 *p;
    u32 *q;
    DispatchFunc f;
    s32 count;
    u32 i;
    u32 b;
    s32 sel;
    s32 field3;
    s32 field5;
    s32 last;
    u32 t;

    seq = arg0->seq;

    libgs_obj_attr_bits0to2 = arg0->flags & 0x7;
    GsLMODE = field3 = (arg0->flags >> 3) & 0x3;
    GsLIGNR = field5 = (arg0->flags >> 5) & 0x1;
    GsLIOFF = (arg0->flags >> 6) & 0x1;
    libgs_obj_attr_bit7 = (arg0->flags >> 7) & 0x1;
    libgs_obj_attr_bit8 = (arg0->flags >> 8) & 0x1;
    GsNDIV = (arg0->flags >> 9) & 0x7;
    GsTRATE = (arg0->flags >> 28) & 0x3;
    GsTON = (arg0->flags >> 30) & 0x1;
    GsDISPON = last = arg0->flags >> 31;

    if (last == 1) {
        return;
    }

    if (field5 == 0) {
        sel = GsLIGHT_MODE;
        if (sel == 1) goto k1;
        if (sel < 2) {
            if (sel == 0) goto k0;
            goto skip;
        }
        if (sel == 2) goto k2;
        if (sel == 3) goto c3;
        goto skip;
    k0:
        f = (DispatchFunc)Gslight_normal;
        goto store;
    k1:
        f = (DispatchFunc)Gslight_normal_fog;
        goto store;
    k2:
        f = (DispatchFunc)Gslight_mate;
        goto store;
    }

    if (field3 == 1) goto c1;
    if (field3 == 0) goto c0;
    if (field3 == 2) goto c2;
    if (field3 == 3) goto c3;
    goto skip;
c0:
    f = (DispatchFunc)Gslight_normal;
    goto store;
c1:
    f = (DispatchFunc)Gslight_normal_fog;
    goto store;
c2:
    f = (DispatchFunc)Gslight_mate;
    goto store;
c3:
    f = (DispatchFunc)Gslight_mate_fog;

store:
    GsLIGHT_FUNC = (s32)f;

skip:
    f = (DispatchFunc)GsLIGHT_FUNC;
    base = (s32 *)arg1[1];
    p = (u32 *)GsOUT_PACKET_P;
    count = f(seq[4], seq[0], seq[2], seq[5]);

    for (i = 0; i < (u32)count; i++) {
        t = p[0];
        t &= 0xFFFFFF;
        t = (t - arg1[2]) >> arg2;
        t <<= 2;
        t += (u32)base;
        q = (u32 *)t;
        b = ((u8 *)p)[3];
        p[0] = (*q & 0xFFFFFF) | (b << 24);
        *q = (*q & 0xFF000000) | ((u32)p & 0xFFFFFF);
        p = (u32 *)((u8 *)p + (b << 2) + 4);
    }
}
