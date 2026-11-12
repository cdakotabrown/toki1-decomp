/* Function: SsUtChangePitch */
/* Address: 0x8008F46C   (the `.sym` records 0x8008F478 / 1060 -- see below) */
/* Size: 276 bytes (69 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @SymbolStart: 0x8008F46C */
/* @SymbolSize: 276 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: remap_stack_slots --map '24:32,20:28' */
/* @PostProc: convert_or_to_addu */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_AS1_NO_FALLTHRU_FILL=1 IDO52_AS1_NO_SHIFT_COMBINE=all IDO52_AS1_NO_LA_PULL=all IDO52_FORCE_WEB_COLOR=0:t0,11:v1 IDO52_FORCE_WEB_COLOR_NOSPLIT=0,11 IDO52_AS1_TIE_TAKE=36 IDO52_AS1_TIE_REJECT=8,9,10,11,80 */
#include "types.h"

typedef struct {
    u8  unk0[8];
    s16 unk8;
    u8  unkA[4];
    s16 unkE;
    s16 unk10;
    s16 unk12;
    u8  unk14[4];
} SpuVoiceParam;

extern SpuVoiceParam _svm_voice[24];
extern u8 _svm_sreg_dirty[24];
extern s16 _svm_sreg_buf[];
extern u8 libsnd_cur_tone_idx;
extern s16 D_80131606;
extern s16 D_8013160A;
extern void SpuVmVSetUp(s32 a, s32 b);
extern s16 note2pitch2(u16 a, u16 b);

s32 SsUtChangePitch(s16 vc, s16 p12, s16 pE, s16 p8, s32 unused, u16 lo, u16 hi)
{
    SpuVoiceParam *t;
    s16 *r;
    s32 idx;

    if (vc < 24) {
        if (vc >= 0) {
            t = &_svm_voice[vc];
            if (p12 == t->unk12) {
                if (pE == t->unkE) {
                    if (p8 == t->unk8) {
                        SpuVmVSetUp(p12, pE);
                        D_80131606 = 32;
                        D_8013160A = vc;
                        libsnd_cur_tone_idx = (u8)t->unk10;
                        idx = vc * 8;
                        _svm_sreg_buf[idx + 2] = note2pitch2(lo, hi);
                        _svm_sreg_dirty[vc] |= 4;
                        return 0;
                    }
                }
            }
        }
    }
    return -1;
}
