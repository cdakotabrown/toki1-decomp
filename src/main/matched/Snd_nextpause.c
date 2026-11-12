/* Address: 0x80091AB0 */
/* Size: 112 bytes (28 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @SymbolStart: 0x80091AB0 */
/* @SymbolSize: 112 */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u8  pad0[43];
    u8  b43;
    u8  pad44[100];
    u32 flags144;
    u8  pad148[20];
} Chan168;

extern Chan168 *_ss_score[];

void Snd_nextpause(s16 bank, s16 ch) {
    Chan168 *c = &_ss_score[bank][ch];

    c->b43 = 0;
    _ss_score[bank][(u32)ch].flags144 &= ~0x100;
}
