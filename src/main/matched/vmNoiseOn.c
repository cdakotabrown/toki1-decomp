/* Address: 0x8008D08C */
/* Size: 932 bytes (233 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 -O2 */
/* `@Compiler: ido52-gas` + `--defer-div` (accepted as of today) + */
/* @CompileFlags: -Wc,-pic0 */
/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=127,16383 IDO52_AS1_XBB_SKIP_CAND=all IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_UGEN_FORCE_TARGET_SAFE_DIV=1 IDO52_AS1_KEEP_ANDI=all IDO52_AS1_NO_SHIFT_COMBINE=all IDO52_UGEN_TEMP_REG_INPLACE=occ:4,5,6 IDO52_COPYPROP_VAR=-56,-60 IDO52_COPYPROP_SITE=lod:21:deny,lod:22:deny IDO52_AS1_NO_PASS1_PROP=11,12,17,18,151,154 IDO52_AS1_NO_COPY_PROP=1 IDO52_FORCE_WEB_COLOR=250:a1,65:t0,39:v1,25:t0,31:t1,37:t2,156:t1,159:t4,5:a2,11:a3,7:a1 IDO52_UGEN_TEMP_REG=4:a1,5:a3,1:t6,2:t7,3:t8,6:v1,7:t9,8:t6,9:t7,10:t8,11:t9,12:t6,13:t7,14:t8,15:t9,16:t6,17:t7,18:t8,19:t9,20:t6,21:t7,22:t8,23:t9,24:t6,25:t7,26:t8,27:t9,28:t6,29:t7,30:t8,31:t6,32:t9,33:t7,34:t8,35:t6,36:t9,37:t7,38:t8,39:t6,40:t9,41:t7,42:t8,43:t6,44:t9,45:t7,46:t8,47:t6,48:t7,49:t9,50:t8 IDO52_AS1_TIE_REJECT=107,139,146,158,170,171,186,209,242,248,266,278,309,367 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
#include "types.h"

typedef struct {
    s16 pad0;
    u16 l;
    u16 r;
    s16 pad3;
} VolEnt;

typedef struct {
    u8  pad[2];
    s16 f2;
    s16 f4;
    u8  pad2[17];
    s8  f23;
} V24b;

extern s16 D_80131606;
extern VolEnt libsnd_seq_vol_tbl[];
extern s8  libsnd_cur_prog_mvol;
extern s8  libsnd_cur_prog_mpan;
extern s8  libsnd_cur_tone_vol;
extern s8  libsnd_cur_tone_pan;
extern s8  D_801315F5;
extern s8  D_801315F2;
extern s8  libsnd_cur_tone_center;
extern s8  libsnd_cur_tone_mode;
extern s16 _svm_stereo_mono;
extern u8 *libsnd_spu_regs_ptr;
extern u8  _svm_sreg_dirty[];
extern s16 _svm_sreg_buf[];
extern V24b _svm_voice[];
extern u16 _svm_vg;
extern u16 libsnd_kon_hi_shadow;
extern u16 _svm_okof1;
extern u16 _svm_okof2;
extern u16 _svm_orev1;
extern u16 _svm_orev2;

void vmNoiseOn(s32 voice)
{
    VolEnt *p;
    u32 lv;
    u32 rv;
    u32 lf;
    u32 rf;
    u32 mv;
    u32 sv;
    u32 pan;
    u32 x;
    u32 k;
    u8 *g;
    u16 w;
    s16 lo;
    s16 hi;
    s32 b0;
    s32 b1;

    p = &libsnd_seq_vol_tbl[D_80131606];
    lv = p->l;
    rv = p->r;
    lv = (lv * 16383) / 127;
    rv = (rv * 16383) / 127;
    mv = libsnd_cur_prog_mvol;
    sv = libsnd_cur_tone_vol;
    pan = libsnd_cur_tone_pan;
    g = libsnd_spu_regs_ptr;
    voice = voice & 0xFF;
    w = *(u16 *)(g + 426);
    lv = (lv * mv) / 127;
    rv = (rv * mv) / 127;
    lv = (lv * sv) / 127;
    rv = (rv * sv) / 127;

    if (pan < 64) {
        rf = (rv * pan) >> 6;
        lf = lv;
    } else {
        k = 127 - pan;
        lf = (lv * k) >> 6;
        rf = rv;
    }
    x = libsnd_cur_prog_mpan;
    if (x < 64) {
        rf = (rf * x) >> 6;
    } else {
        lf = (lf * x) >> 6;
    }
    x = D_801315F5;
    if (x < 64) {
        rf = (x * rf) >> 6;
    } else {
        k = 127 - x;
        lf = (lf * k) >> 6;
    }
    if (_svm_stereo_mono == 1) {
        if (lf < rf) {
            lf = rf;
        } else {
            rf = lf;
        }
    }

    w = w & 0xC0FF;
    w = w | (u16)(((D_801315F2 - libsnd_cur_tone_center) & 0x3F) << 8);
    *(u16 *)(g + 426) = w;
    _svm_sreg_buf[(s16)(voice * 8)] = lf;
    _svm_sreg_dirty[voice] |= 3;
    _svm_sreg_buf[(s16)(voice * 8) + 1] = rf;

    if ((u32)voice < 16) {
        lo = 1 << voice;
        hi = 0;
    } else {
        hi = 1 << (voice + 16);
        lo = 0;
    }
    b0 = lo;
    b1 = hi;
    _svm_voice[voice].f4 = 10;
    _svm_voice[voice].f23 = 1;
    _svm_vg = _svm_vg | (u16)b0;
    libsnd_kon_hi_shadow = libsnd_kon_hi_shadow | (u16)b1;
    _svm_okof1 = _svm_okof1 & ~_svm_vg;
    _svm_okof2 = _svm_okof2 & ~libsnd_kon_hi_shadow;
    if (libsnd_cur_tone_mode & 4) {
        _svm_orev1 = _svm_orev1 | (u16)b0;
        _svm_orev2 = _svm_orev2 | (u16)b1;
    } else {
        _svm_orev1 = _svm_orev1 & (u16)~lo;
        _svm_orev2 = _svm_orev2 & (u16)~hi;
    }
    *(s16 *)(g + 404) = b0;
    *(s16 *)(g + 406) = b1;
}
