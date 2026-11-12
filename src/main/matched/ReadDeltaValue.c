/* Address: 0x80094A1C */
/* Size: 176 bytes (44 instructions) */
/* Region: Library (0x80086000+) */
/* `@Compiler: ido52-gas` (45 words, 2/44) and `ido52-aspsx` (49 words, */
/* @SymbolStart: 0x80094A1C */
/* @SymbolSize: 176 */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_AS1_NO_COPY_PROP=all IDO52_UGEN_TEMP_REG=6:t2,7:t3 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    s32 f0;
    u8 *ptr;
    u8 pad8[120];
    s32 time;
    u8 pad84[36];
} Chan;

extern Chan *_ss_score[];

s32 ReadDeltaValue(s16 port, s16 ch)
{
    Chan *c;
    s32 v;
    s32 b;
    s32 d;

    c = &_ss_score[port][ch];
    v = *c->ptr++;
    if (v == 0) {
        return v;
    }
    if (v & 0x80) {
        v &= 0x7F;
        do {
            b = *c->ptr;
            v = (v << 7) + (b & 0x7F);
            c->ptr++;
        } while (b & 0x80);
    }
    d = v * 10;
    c->time += d;
    return d;
}
