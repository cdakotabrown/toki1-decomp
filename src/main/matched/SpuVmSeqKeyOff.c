/* Address: 0x8008E950 */
/* Size: 156 bytes (39 instructions) */
/* Region: Library (0x80086000+) */
/* Compiler: IDO 5.2 -O2 -cckr, as1 + convert_or_to_addu */
/* @CompileFlags: -cckr -Wo,-do_opt_saved_regs */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_CONST_NOINREG_VALUES=24 IDO52_AS1_NO_LA_PULL=0 IDO52_AS1_TIE_REJECT=13,14 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u8 pad0[10];
    s16 id;
    u8 pad12[12];
} Slot;

extern s8 spuVmMaxVoice;
extern Slot _svm_voice[];
extern s16 D_8013160A;
void SpuVmKeyOffNow();

void SpuVmSeqKeyOff(s16 id)
{
    u8 i;

    for (i = 0; i < spuVmMaxVoice; i++) {
        if (_svm_voice[i].id == id) {
            D_8013160A = i;
            SpuVmKeyOffNow(0);
        }
    }
}
