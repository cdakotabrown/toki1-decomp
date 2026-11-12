/* Address: 0x8008FFE0 (true entry; 0x8008FFF8 is the prologue, `head`) */
/* Size: 312 bytes (78 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @SymbolStart: 0x8008FFE0 */
/* @SymbolSize: 312 */
/* @CompileFlags: -Wc,-pic0 */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_AS1_KEEP_SEXT=2 IDO52_FORCE_WEB_COLOR=5:t1,17:t0 IDO52_AS1_TIE_TAKE=38,59 IDO52_UGEN_TEMP_REG=13:t3,20:t9 IDO52_AS1_NO_FALLTHRU_FILL=all IDO52_AS1_NO_LA_PULL=all IDO52_AS1_NO_COPY_PROP=0 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
#include "types.h"

typedef struct {
    u8  pad0[43];
    u8  b43;
    u8  pad44[24];
    s16 f68;
    s16 f70;
    u8  pad72[42];
    u16 f114;
    u16 f116;
    u8  pad118[26];
    u32 flags144;
    u8  pad148[20];
} Chan168;

extern Chan168 *_ss_score[];
extern void SpuVmSetSeqVol(s32 a0, u16 a1, u16 a2, s32 a3);

void Snd_SetPlayMode(s32 arg0, s32 arg1, s8 mode, s16 val)
{
    Chan168 *c;
    Chan168 *d;
    s32 bank;
    s32 ch;

    ch = (arg1 << 16) >> 16;
    bank = (arg0 << 16) >> 16;
    d = &_ss_score[bank][ch];
    d->flags144 &= ~0x200;
    c = d;
    d = &_ss_score[bank][(u32)ch];
    d->flags144 &= ~0x4;
    c->f68 = val;
    if (mode == 1) {
        d = (Chan168 *)((u8 *)_ss_score[bank] + ((ch * 21) << 3));
        d->flags144 |= 0x1;
        c->f70 = 0;
        c->b43 = 1;
        SpuVmSetSeqVol((ch << 8) | bank, c->f114, c->f116, 0);
    } else if (mode == 0) {
        d = (Chan168 *)((u8 *)_ss_score[bank] + (((u32)ch * 21) << 3));
        d->flags144 |= 0x2;
    }
}
