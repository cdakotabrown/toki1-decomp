/* Address: 0x8008C9A8 */
/* Size: 880 bytes (212 instructions) */
/* Region: Library (0x80086000-) */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count auto */
/* @CompileFlags: -cckr -Wc,-pic0 */
/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=16129,127:1:2,16383 IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_UGEN_FORCE_TARGET_SAFE_DIV=1 IDO52_AS1_KEEP_ANDI=occ:6,7 IDO52_FORCE_WEB_COLOR=108:a3,29:a0,51:a1,106:t0 IDO52_UGEN_TEMP_REG=41:v0,45:a1,21:t5,22:t6,29:t2,30:t3,31:t4 IDO52_UGEN_TEMP_REG_INPLACE=occ:41,45 IDO52_AS1_TIE_REJECT=156,187,189,193,194,197,198,285 IDO52_AS1_XBB_SKIP_CAND=all IDO52_AS1_NO_FALLTHRU_FILL=1,2 */
#include "types.h"

typedef struct {
    u8 pad0[24];
    u8 vol;
} SndSeq;

typedef struct {
    u16 f0;
    u16 l;
    u16 r;
    u16 f6;
} VolEnt;

typedef struct {
    u8 pad0[4];
    u16 pitch;
    u8 pad6[17];
    s8 dirty;
} VoiceAttr;

extern SndSeq *_svm_vh;
extern s8 D_801315F4;
extern s8 libsnd_cur_prog_mvol;
extern s8 libsnd_cur_tone_vol;
extern s16 D_80131606;
extern u8 libsnd_cur_tone_pan;
extern u8 libsnd_cur_prog_mpan;
extern u8 D_801315F5;
extern s16 _svm_stereo_mono;
extern s16 D_8013160A;
extern s8 libsnd_cur_tone_mode;
extern VolEnt libsnd_seq_vol_tbl[];
extern u16 _svm_sreg_buf[];
extern u8 _svm_sreg_dirty[];
extern VoiceAttr _svm_voice[];
extern u16 _svm_orev1;
extern u16 _svm_orev2;
extern u16 _svm_okof1;
extern u16 _svm_okof2;
extern u16 _svm_vg;
extern u16 libsnd_kon_hi_shadow;

void SpuVmKeyOnNow(arg0, pitch)
int arg0;
u16 pitch;
{
    VolEnt *e;
    int vol;
    int l;
    u32 r;
    int vr;
    u32 pan;
    s16 ch;
    u16 *p;
    VoiceAttr *va;
    u8 *fl;
    u16 lo;
    u16 hi;
    u32 nlo;
    u32 nhi;

    vol = (D_801315F4 * 0x3FFF) * _svm_vh->vol / 16129;
    vol = (u32)vol * libsnd_cur_prog_mvol * libsnd_cur_tone_vol / 16129;
    e = &libsnd_seq_vol_tbl[D_80131606];
    vr = vol;
    vol = vol * e->l / 127;
    vr = vr * e->r / 127;
    pan = libsnd_cur_tone_pan;
    if (pan < 64) {
        r = (vr * pan) >> 6;
        l = vol;
    } else {
        l = (vol * (127 - pan)) >> 6;
        r = vr;
    }
    pan = libsnd_cur_prog_mpan;
    if (pan < 64) {
        r = (r * pan) >> 6;
    } else {
        l = (l * (127 - pan)) >> 6;
    }
    pan = D_801315F5;
    if (pan < 64) {
        r = (r * pan) >> 6;
    } else {
        l = (l * (127 - pan)) >> 6;
    }
    if (_svm_stereo_mono == 1) {
        if (l < r) {
            l = r;
        } else {
            r = l;
        }
    }
    ch = D_8013160A;
    fl = &_svm_sreg_dirty[ch];
    p = &_svm_sreg_buf[(u16)(ch * 8)];
    p[2] = pitch;
    va = &_svm_voice[ch];
    va->pitch = pitch;
    va->dirty = 1;
    *fl |= 7;
    l = (u32)(l * l) / 16383;
    p[0] = l;
    r = r * r / 16383;
    p[1] = r;
    if (ch < 16) {
        lo = 1 << ch;
        hi = 0;
    } else {
        hi = 1 << (ch + 16);
        lo = 0;
    }
    if (libsnd_cur_tone_mode & 4) {
        _svm_orev1 |= lo;
        _svm_orev2 |= hi;
    } else {
        _svm_orev1 &= ~lo;
        _svm_orev2 &= ~hi;
    }
    nlo = (_svm_vg | lo) & 0xFFFF;
    nhi = (libsnd_kon_hi_shadow | hi) & 0xFFFF;
    _svm_okof1 &= ~nlo;
    _svm_okof2 &= ~nhi;
    _svm_vg = nlo;
    libsnd_kon_hi_shadow = nhi;
}
