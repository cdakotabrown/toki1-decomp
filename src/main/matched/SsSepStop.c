/* Address: 0x800901B4 (true entry; the recorded 0x800901CC is the prologue) */
/* Size: 236 bytes (59 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @SymbolStart: 0x800901B4 */
/* @SymbolSize: 236 */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_UGEN_TEMP_REG=13:t7,18:t1 IDO52_AS1_TIE_TAKE=30,47 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u8  pad0[144];
    u32 flags144;
    u8  pad148[20];
} Chan168;

extern Chan168 *_ss_score[];
extern void Snd_stop(void);

void SsSepStop(s16 bank, s16 ch)
{
    Chan168 *c;

    c = &_ss_score[bank][ch];
    c->flags144 &= ~1;
    c = &_ss_score[bank][(u32)ch];
    c->flags144 &= ~2;
    c = (Chan168 *)((u8 *)_ss_score[bank] + ((ch * 21) << 3));
    c->flags144 &= ~8;
    c = (Chan168 *)((u8 *)_ss_score[bank] + (((u32)ch * 21) << 3));
    c->flags144 |= 4;
    Snd_stop();
}
