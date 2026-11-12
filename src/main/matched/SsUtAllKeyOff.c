/* Function: SsUtAllKeyOff */
/* Address: 0x8008F89C */
/* Size: 300 bytes (75 instructions) */
/* Region: Library (0x80086000+) -- Konami sound driver, sibling of SsSepOpen */
/* Compiler: IDO 5.2 -O2 (confirmed: t6..t9 expression temps in ugen's */
/* @SymbolStart: 0x8008F89C */
/* @SymbolSize: 300 */
/* @CompileFlags: -Wo,-do_opt_saved_regs */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_CONST_NOINREG_VALUES=24:1:1 IDO52_AS1_NO_SHIFT_COMBINE=all IDO52_AS1_TIE_SRC_ORDER=bb:2 IDO52_AS1_TIE_TAKE=23,31,39,175 IDO52_AS1_TIE_REJECT=6,62,63,182,185 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
#include "types.h"

typedef struct {
    s16 a;
    u16 b;
    s16 c;
    s16 d;
    s16 e;
    s16 f;
    s16 g;
    s16 h;
    s16 i;
    s16 j;
    s16 k;
    u8 l;
    u8 m;
} Ent;

extern Ent _svm_voice[];
extern u16 *libsnd_spu_regs_ptr;
extern s8 spuVmMaxVoice;
extern s16 D_8013160A;

extern void SpuVmKeyOffNow(s32 arg0);

void SsUtAllKeyOff(s32 arg0) {
    s16 i;
    u16 *spuData;

    i = 0;
    if (spuVmMaxVoice > 0) {
        spuData = libsnd_spu_regs_ptr;
        do {
            _svm_voice[i].b = 24;
            _svm_voice[i].a = 255;
            _svm_voice[i].m = 0;
            _svm_voice[i].c = 0;
            _svm_voice[i].d = 0;
            _svm_voice[i].f = 255;
            _svm_voice[i].g = 0;
            _svm_voice[i].h = 0;
            _svm_voice[i].i = 255;
            spuData[(s16)(i * 8) + 0] = 0;
            spuData[(s16)(i * 8) + 1] = 0;
            spuData[(s16)(i * 8) + 3] = 512;
            spuData[(s16)(i * 8) + 2] = 4096;
            spuData[(s16)(i * 8) + 4] = 0x80FF;
            spuData[(s16)(i * 8) + 5] = 16384;

            D_8013160A = i;
            SpuVmKeyOffNow(1);

            i = (s16)(i + 1);
        } while (i < spuVmMaxVoice);
    }
}
