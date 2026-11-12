/* Address: 0x8008E660 */
/* Size: 368 bytes (92 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 -O2 */
/* `@CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all` makes IDO emit */
/* @CompileFlags: -Wc,-pic0 -cckr */
/* @CompileEnv: IDO52_AS1_KEEP_SEXT=all IDO52_AS1_NO_SHIFT_COMBINE=all IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_CONST_NOINREG_VALUES=24,16383,127 IDO52_AS1_NO_FALLTHRU_FILL=1 IDO52_AS1_NO_LA_PULL=1 IDO52_AS1_TIE_TAKE=36,38 IDO52_AS1_TIE_REJECT=17 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
#include "types.h"

typedef struct {
    u16 pad0;
    u16 l;
    u16 r;
    u16 pad3;
} VolEnt;

typedef struct {
    u8  pad[10];
    s16 chan;
    u8  pad2[12];
} VoiceEnt;

extern VolEnt libsnd_seq_vol_tbl[];
extern s16 D_80131606;
extern s8  spuVmMaxVoice;
extern VoiceEnt _svm_voice[];
extern u16 _svm_sreg_buf[];
extern volatile u8 _svm_sreg_dirty[];

s16 SpuVmSetSeqVol(a0, a1, a2, a3)
s16 a0;
u16 a1, a2;
s16 a3;
{
    int t;
    int id;
    VolEnt *ve;
    s16 i;
    u16 *pan;

    t = a0 & 0xFF;
    id = (t << 16) >> 16;
    if (id < 34) {
        ve = &libsnd_seq_vol_tbl[id];
        ve->l = a1;
        ve->r = a2;
        D_80131606 = id;
        if (ve->l >= 128) {
            ve->l = 127;
        }
        if (ve->r >= 128) {
            ve->r = 127;
        }
        if (a3 == 1) {
            for (i = 0; i < spuVmMaxVoice; i++) {
                if (t == (_svm_voice[i].chan & 0xFF)) {
                    pan = &_svm_sreg_buf[(s16)(i * 8)];
                    pan[0] = a1 * 16383 / 127;
                    *(volatile u16*)&pan[1] = a2 * 16383 / 127;
                    _svm_sreg_dirty[i] |= 3;
                }
            }
        }
        return t;
    }
    D_80131606 = id;
    goto Lr;
Lr:
    return -1;
}
