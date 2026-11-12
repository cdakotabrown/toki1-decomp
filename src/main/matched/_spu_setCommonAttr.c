/* Address: 0x80097024 */
/* Size: 856 bytes (214 instructions) */
/* Region: Library (0x80086000+) */
/* Compiler: IDO 5.2 -O2 codegen, PSY-Q-style assembly (@Compiler: ido52-gas) */
/* @Compiler: ido52-gas */
/* @CompileFlags: --ido-fill-load-delay --ido-copy-and-bump */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u32 mask;
    u16 mvolL;
    u16 mvolR;
    s16 rvolL;
    s16 rvolR;
    u16 cdvolL;
    u16 cdvolR;
    s32 cdReverb;
    s32 cdMix;
    u16 extvolL;
    u16 extvolR;
    s32 extReverb;
    s32 extMix;
    u16 noiseClk;
    u16 reverbOn;
    u16 xferMode;
    u16 xferBusy;
    u32 endx;
    u16 stat;
} SpuCommonRegs;

extern volatile u16 * volatile _spu_RXX;

s32 _spu_setCommonAttr(SpuCommonRegs *attr)
{
    u32 mask;
    s32 all;
    volatile u16 *r;

    mask = attr->mask;
    if (mask == 0) {
        all = 1;
    } else {
        all = 0;
    }

    if (!(all || (mask & 0x1))) {
        r = _spu_RXX;
    } else {
        r = _spu_RXX;
        r[0xC0] = attr->mvolL;
    }
    if (all || (mask & 0x2)) {
        r[0xC1] = attr->mvolR;
    }
    if (all || (mask & 0x4)) {
        r[0xC2] = attr->rvolL;
    }
    if (all || (mask & 0x8)) {
        r[0xC3] = attr->rvolR;
    }
    if (all || (mask & 0x10)) {
        r[0xD8] = attr->cdvolL;
    }
    if (all || (mask & 0x20)) {
        r[0xD9] = attr->cdvolR;
    }
    if (all || (mask & 0x100)) {
        if (!attr->cdReverb) {
            r[0xD5] &= 0xFFFB;
        } else {
            r[0xD5] |= 0x4;
        }
    }
    if (all || (mask & 0x400)) {
        if (!attr->cdMix) {
            r[0xD5] &= 0xFFFE;
        } else {
            r[0xD5] |= 0x1;
        }
    }
    if (all || (mask & 0x40)) {
        r[0xDA] = attr->extvolL;
    }
    if (all || (mask & 0x80)) {
        r[0xDB] = attr->extvolR;
    }
    if (all || (mask & 0x200)) {
        if (!attr->extReverb) {
            r[0xD5] &= 0xFFF7;
        } else {
            r[0xD5] |= 0x8;
        }
    }
    if (all || (mask & 0x800)) {
        if (!attr->extMix) {
            r[0xD5] &= 0xFFFD;
        } else {
            r[0xD5] |= 0x2;
        }
    }
    if (all || (mask & 0x1000)) {
        r[0xD5] &= 0xC0FF;
        r[0xD5] |= (u16)((attr->noiseClk & 0x3F) << 8);
    }
    if (all || (mask & 0x2000)) {
        r[0xD5] &= 0xFF7F;
        r[0xD5] |= (u16)((attr->reverbOn & 0x1) << 7);
    }
    if (all || (mask & 0x4000)) {
        r[0xD6] &= 0xFFF1;
        r[0xD6] |= (u16)((attr->xferMode & 0x7) << 1);
    }
    if (all || (mask & 0x8000)) {
        r[0xD6] &= 0xFFFE;
        r[0xD6] |= (u16)(attr->xferBusy & 0x1);
    }
    return 0;
}
