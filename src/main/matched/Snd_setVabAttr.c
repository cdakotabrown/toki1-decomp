/* Address: 0x80094078 */
/* Size: 1308 bytes (327 instructions) -- the recorded 1352 runs into the */
/* Region: Library (0x80086000+), IDO 5.2 -O2 */
/* @SymbolStart: 0x80094078 */
/* @SymbolSize: 1308 */
/* @CompileEnv: IDO52_COPYPROP_SITE=lod:1:deny IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_AS1_NO_FALLTHRU_FILL=0,5 IDO52_AS1_KEEP_JUMP_NEXT=all IDO52_KEEP_JUMP_NEXT=all IDO52_AS1_NO_BOPT=occ:0 IDO52_AS1_XBB_SKIP_CAND=0,1,2,3,5,6 */
/* @CompileFlags: -cckr -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: remap_stack_slots --map 20:36 */
/* @PostProc: convert_or_to_addu */
#include "types.h"

typedef struct {
    u8 b0;
    u8 b1;
    u8 b2;
    u8 b3;
    u8 b4;
    u8 b5;
    u8 b6;
    u8 b7;
    u8 b8;
    u8 b9;
    u8 b10;
    u8 b11;
    u8 b12[4];
    u16 u16_;
    u16 u18;
    u8 b20[12];
} SA;

typedef struct {
    s16 h[9];
} SB;

extern void SsUtGetVagAtr(s16, s16, s16, SA *);
extern void SsUtSetVagAtr(s16, s16, s16, SA *);
extern void SsUtResolveADSR(u16, u16, SB *);
extern void SsUtBuildADSR(SB *, u16 *, u16 *);
extern void SsUtReverbOff(void);
extern void SsUtReverbOn(void);
extern void SsUtSetReverbType(s16);
extern void SsUtSetReverbDepth();
extern void SsUtSetReverbFeedback(s16);
extern void SsUtSetReverbDelay(s16);

void Snd_setVabAttr(ch, patch, idx, a, b, sel, val)
s16 ch;
s16 patch;
s16 idx;
SA a;
SB b;
s16 sel;
u8 val;
{
    int x;
    s16 h;

    SsUtGetVagAtr(ch, patch, idx, &a);
    x = sel;
    switch (x) {
    case 0:
        a.b0 = val;
        SsUtSetVagAtr(ch, patch, idx, &a);
        break;
    case 1:
        a.b1 = val;
        SsUtSetVagAtr(ch, patch, idx, &a);
        if (val == 0) {
            SsUtReverbOff();
        } else if (val == 1) {
        } else if (val == 2) {
        } else if (val == 3) {
        } else if (val == 4) {
            SsUtReverbOn();
        }
        break;
    case 2:
        a.b6 = val;
        SsUtSetVagAtr(ch, patch, idx, &a);
        break;
    case 3:
        a.b7 = val;
        SsUtSetVagAtr(ch, patch, idx, &a);
        break;
    case 4:
        SsUtResolveADSR(a.u16_, a.u18, &b);
        b.h[5] = 0;
        b.h[0] = val;
        SsUtBuildADSR(&b, &a.u16_, &a.u18);
        SsUtSetVagAtr(ch, patch, idx, &a);
        break;
    case 5:
        SsUtResolveADSR(a.u16_, a.u18, &b);
        b.h[5] = 1;
        b.h[0] = val;
        SsUtBuildADSR(&b, &a.u16_, &a.u18);
        SsUtSetVagAtr(ch, patch, idx, &a);
        break;
    case 6:
        SsUtResolveADSR(a.u16_, a.u18, &b);
        b.h[1] = val;
        SsUtBuildADSR(&b, &a.u16_, &a.u18);
        SsUtSetVagAtr(ch, patch, idx, &a);
        break;
    case 7:
        SsUtResolveADSR(a.u16_, a.u18, &b);
        b.h[2] = val;
        SsUtBuildADSR(&b, &a.u16_, &a.u18);
        SsUtSetVagAtr(ch, patch, idx, &a);
        break;
    case 8:
        SsUtResolveADSR(a.u16_, a.u18, &b);
        b.h[6] = 0;
        b.h[3] = val;
        SsUtBuildADSR(&b, &a.u16_, &a.u18);
        SsUtSetVagAtr(ch, patch, idx, &a);
        break;
    case 9:
        SsUtResolveADSR(a.u16_, a.u18, &b);
        b.h[6] = 1;
        b.h[3] = val;
        SsUtBuildADSR(&b, &a.u16_, &a.u18);
        SsUtSetVagAtr(ch, patch, idx, &a);
        break;
    case 10:
        SsUtResolveADSR(a.u16_, a.u18, &b);
        b.h[7] = 0;
        b.h[4] = val;
        SsUtBuildADSR(&b, &a.u16_, &a.u18);
        SsUtSetVagAtr(ch, patch, idx, &a);
        break;
    case 11:
        SsUtResolveADSR(a.u16_, a.u18, &b);
        b.h[7] = 1;
        b.h[4] = val;
        SsUtBuildADSR(&b, &a.u16_, &a.u18);
        SsUtSetVagAtr(ch, patch, idx, &a);
        break;
    case 12:
        SsUtResolveADSR(a.u16_, a.u18, &b);
        if (val != 0 && val < 64) {
            b.h[8] = 0;
        } else if (val >= 64 && val < 128) {
            b.h[8] = 1;
        }
        SsUtBuildADSR(&b, &a.u16_, &a.u18);
        SsUtSetVagAtr(ch, patch, idx, &a);
        break;
    case 13:
        SsUtResolveADSR(a.u16_, a.u18, &b);
        a.b9 = val;
        SsUtBuildADSR(&b, &a.u16_, &a.u18);
        SsUtSetVagAtr(ch, patch, idx, &a);
        break;
    case 14:
        SsUtResolveADSR(a.u16_, a.u18, &b);
        a.b10 = val;
        SsUtBuildADSR(&b, &a.u16_, &a.u18);
        SsUtSetVagAtr(ch, patch, idx, &a);
        break;
    case 15:
        SsUtSetReverbType(val);
        break;
    case 16:
        h = val;
        SsUtSetReverbDepth(h, h);
        break;
    case 17:
        SsUtSetReverbFeedback(val);
        break;
    case 18:
        SsUtSetReverbDelay(val);
        break;
    case 19:
        SsUtSetReverbDelay(val);
        break;
    case 20:
    case 21:
    case 22:
        break;
    }
}
