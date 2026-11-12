/* Address: 0x80092F18 */
/* Size: 116 bytes (29 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 native as1, stock pic0 */
/* @SymbolStart: 0x80092F18 */
/* @SymbolSize: 116 */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_AS1_KEEP_ANDI=all */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u8  pad0[18];
    u8  chan;
    u8  pad19[25];
    u8  prog[21];
    u8  pad65[71];
    s32 slot;
    u8  pad140[28];
} SeqRec;

extern SeqRec *_ss_score[];
extern s32 ReadDeltaValue(s16, s16);

void SetProgramChange(s16 ch, s16 seq, u8 prog)
{
    s32 idx;
    SeqRec *rec;

    rec = &_ss_score[ch][seq];
    idx = rec->chan;
    rec->prog[idx] = prog;
    rec->slot = ReadDeltaValue(ch, seq);
}
