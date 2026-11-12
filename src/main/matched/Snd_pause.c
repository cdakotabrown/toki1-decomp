/* Address: 0x80091A10 (recorded symbol 0x80091A24 is the prologue; the */
/* Size: 160 bytes (40 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @SymbolStart: 0x80091A10 */
/* @SymbolSize: 160 */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_FORCE_WEB_COLOR_NOSPLIT=10 IDO52_AS1_TIE_TAKE=17,29,33,48 IDO52_AS1_TIE_REJECT=1,12,21,24,36 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: remap_stack_slots --map '20:28,28:32,32:36' */
#include "types.h"

typedef struct {
    u8  pad0[43];
    u8  b43;
    u8  pad44[100];
    u32 flags144;
    u8  pad148[20];
} Chan168;

extern Chan168 *_ss_score[];
extern void SpuVmSeqKeyOff(s16 arg0);

void Snd_pause(s16 bank, s16 ch) {
    Chan168 **tbl = &_ss_score[bank];
    Chan168 *rec = &(*tbl)[ch];

    SpuVmSeqKeyOff(bank);
    rec->b43 = 0;
    (*tbl)[(u32)ch].flags144 &= ~2;
}
