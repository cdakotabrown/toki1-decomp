/* Address: 0x8008E0E4 */
/* Size: 828 bytes (207 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @CompileFlags: -Wc,-pic0 -Wo,-do_opt_saved_regs -cckr */
/* @CompileEnv: IDO52_AS1_TIE_TAKE=395,403 IDO52_AS1_TIE_REJECT=65,136,345,43,44,399 IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_CONST_NOINREG_VALUES=24 IDO52_AS1_NO_LA_PULL=all IDO52_AS1_NO_FALLTHRU_FILL=all IDO52_AS1_NO_PASS1_PROP=all IDO52_LDA_NOINREG_CLASSES=1 IDO52_CFE_FRAME_TOP_BASE=all */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: remap_stack_slots --keep-frame --map 20:28,24:32,28:36,32:40,36:44,44:52,48:56,52:60,64:76,68:80 */
/* @PostProc: swap_instructions --offset1 0x264 --offset2 0x26c */
/* @PostProc: swap_instructions --offset1 0x270 --offset2 0x274 */
/* @PostProc: swap_instructions --offset1 0x274 --offset2 0x278 */
#include "types.h"

typedef struct {
    u8  b[16];
} ProgAtr;

typedef struct {
    u8  b[10];
    u8  pad_0a[12];
    s16 h16;
    u8  pad_18[8];
} ToneAtr;

extern s8 spuVmMaxVoice;

typedef struct {
    u8  f0;
    s8  f1;
    s8  f2;
    s8  f3;
    u8  f4;
    u8  f5;
    u8  pad_f6;
    s8  f7;
    u8  pad_f8[2];
    u8  fa;
    u8  fb;
    s8  fc;
    u8  fd;
    u8  fe;
    s8  ff;
    u8  b00;
    u8  b01;
    u8  b02;
    u8  b03;
    s8  b04;
    u8  pad_05;
    s16 h06;
    s16 h08;
    s16 h0a;
} SndState;

extern SndState _svm_cur;
extern u8 *_svm_vh;
extern ProgAtr *_svm_pg;
extern ToneAtr *_svm_tn;

typedef struct {
    s16 field_00;
    s16 field_02;
    u8  field_04;
    u8  field_05;
    u8  field_06;
    u8  field_07;
    s16 field_08;
    s16 field_0a;
    s16 field_0c;
    s16 field_0e;
    s16 field_10;
    s16 field_12;
    s16 field_14;
    u8  field_16[2];
} SndEntry;

extern SndEntry _svm_voice[];

extern s32 SpuVmVSetUp();
extern void SpuVmKeyOff();
extern s32 SpuVmSelectToneAndVag();
extern s32 SpuVmAlloc();
extern void SpuVmDoAllocate();
extern void vmNoiseOn();
extern s32 note2pitch();
extern void SpuVmKeyOnNow();

s32 SpuVmKeyOn(a0, a1, a2, a3, a4, a5)
    short a0, a1, a2;
    unsigned short a3, a4, a5;
{
    u8 pad[8];
    u8 buf2[128];
    u8 buf1[128];
    ToneAtr *entry;
    ProgAtr *ptr;
    SndEntry *snd;
    s32 ret;
    unsigned char i;
    unsigned char n;
    s32 d;

    if (SpuVmVSetUp(a1, a2) != 0) {
        return -1;
    }

    _svm_cur.h06 = a0 & 0xFF;
    d = a3;
    _svm_cur.f2 = d;
    _svm_cur.f3 = 0;
    _svm_cur.f4 = a4;
    _svm_cur.f5 = a5;

    ptr = _svm_pg;
    ptr += a2;
    _svm_cur.fa = ptr->b[1];
    _svm_cur.fb = ptr->b[4];
    _svm_cur.f0 = ptr->b[0];

    ret = 0;
    if ((u32)_svm_cur.f7 >= *(u16 *)(_svm_vh + 18)) {
        return -1;
    }

    if (a4 == 0) {
        SpuVmKeyOff(a0, a1, a2, a3);
    } else {
        n = SpuVmSelectToneAndVag(buf1, buf2);
        for (i = 0; i < n; i++) {
            _svm_cur.h08 = buf2[i];
            _svm_cur.fc = buf1[i];
            entry = _svm_tn;
            entry += (u16)(_svm_cur.f7 * 16 + _svm_cur.fc);
            _svm_cur.ff = entry->b[0];
            _svm_cur.fd = entry->b[2];
            _svm_cur.fe = entry->b[3];
            _svm_cur.b00 = entry->b[4];
            _svm_cur.b01 = entry->b[5];
            _svm_cur.b04 = entry->b[1];
            _svm_cur.b02 = entry->b[6];
            _svm_cur.b03 = entry->b[7];

            _svm_cur.h0a = SpuVmAlloc(0);
            if (_svm_cur.h0a < spuVmMaxVoice) {
                snd = _svm_voice;
                snd += _svm_cur.h0a;
                snd->field_16[1] = 1;
                snd->field_02 = 0;
                snd->field_0a = a0;
                snd->field_12 = _svm_cur.f1;
                snd->field_0e = a2;
                snd->field_0c = _svm_cur.f7;
                snd->field_08 = d;
                snd->field_10 = _svm_cur.fc;
                snd->field_14 = _svm_cur.ff;
                snd->field_00 = _svm_cur.h08;
                SpuVmDoAllocate();
                if (_svm_cur.h08 == 255) {
                    vmNoiseOn((u8)_svm_cur.h0a);
                } else {
                    SpuVmKeyOnNow(n, (u16)note2pitch());
                }
                ret = (ret << 4) | _svm_cur.h0a;
            }
        }
    }
    return ret;
}
