/* Region: Library (0x80086000+), IDO 5.2 */
/* Address: 0x8008FEF4 */
/* Size: 236 bytes (59 instructions) */
/* @SymbolStart: 0x8008FEF4 */
/* @SymbolSize: 236 */
/* @CompileFlags: -Wc,-pic0 */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_FORCE_WEB_COLOR=18:t0 IDO52_AS1_KEEP_SEXT=0,5 IDO52_AS1_NO_COPY_PROP=1,5 IDO52_UGEN_TEMP_REG=5:t4,8:t7,6:t5,7:t6,9:t8,12:t5,10:t9,11:t4,13:t5,14:t6,15:t7,17:t3 IDO52_AS1_TIE_TAKE=38,59 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
#include "types.h"

typedef struct {
    u8  pad0[4];
    s32 f4;
    s32 f8;
    u8  pad12[56];
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

void SsPlayBack(s32 arg0, s32 arg1, s32 pitch)
{
    Chan168 *c;
    Chan168 *d;
    s16 bank;
    s16 ch;

    ch = (arg1 << 16) >> 16;
    bank = (arg0 << 16) >> 16;
    d = &_ss_score[bank][ch];
    d->flags144 &= ~0x200;
    c = d;
    d = &_ss_score[bank][(u32)ch];
    d->flags144 &= ~0x4;
    d = (Chan168 *)((u8 *)_ss_score[bank] + ((ch * 21) << 3));
    d->flags144 |= 0x1;
    c->f68 = (pitch << 16) >> 16;
    c->f70 = 0;
    c->f4 = c->f8;
    goto call;
call:
    SpuVmSetSeqVol((ch << 8) | bank, c->f114, c->f116, 0);
}
