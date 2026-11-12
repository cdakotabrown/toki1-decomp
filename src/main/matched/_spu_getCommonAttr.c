/* Address: 0x8009737C */
/* Size: 436 bytes (109 instructions) */
/* Region: Library (0x80086000+) */
/* Compiler: IDO 5.2 -O2 codegen, PSY-Q-style assembly (@Compiler: ido52-gas) */
/* @Compiler: ido52-gas */
/* @CompileFlags: --ido-fill-load-delay */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u32 mask;
    u16 mvolL;
    u16 mvolR;
    u16 rvolL;
    u16 rvolR;
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

s32 _spu_getCommonAttr(SpuCommonRegs *attr)
{
    volatile u16 *r = _spu_RXX;
    s32 ret;

    attr->mvolL = r[0xC0] & 0xFFFF;
    attr->mvolR = r[0xC1] & 0xFFFF;
    attr->rvolL = r[0xC2] & 0xFFFF;
    attr->rvolR = r[0xC3] & 0xFFFF;
    attr->cdvolL = r[0xD8] & 0xFFFF;
    attr->cdvolR = r[0xD9] & 0xFFFF;
    if (r[0xD5] & 4) {
        attr->cdReverb = 1;
    } else {
        attr->cdReverb = 0;
    }
    if (r[0xD5] & 1) {
        attr->cdMix = 1;
    } else {
        attr->cdMix = 0;
    }
    attr->extvolL = r[0xDA] & 0xFFFF;
    attr->extvolR = r[0xDB] & 0xFFFF;
    if (r[0xD5] & 8) {
        attr->extReverb = 1;
    } else {
        attr->extReverb = 0;
    }
    if (r[0xD5] & 2) {
        attr->extMix = 1;
    } else {
        attr->extMix = 0;
    }
    attr->noiseClk = (r[0xD5] & 0x3F00u) >> 8;
    if (r[0xD5] & 0x80) {
        attr->reverbOn = 1;
    } else {
        attr->reverbOn = 0;
    }
    attr->xferMode = (r[0xD6] & 0xEu) >> 1;
    if (r[0xD6] & 1) {
        attr->xferBusy = 1;
    } else {
        attr->xferBusy = 0;
    }
    attr->endx = (r[0xCE] & 0xFFFF) + ((r[0xCF] & 0xFFFF) << 16);
    ret = 0;
    attr->stat = r[0xD7] & 0xFFFF;
    return ret;
}
