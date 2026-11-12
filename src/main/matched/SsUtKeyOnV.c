/* @SymbolStart: 0x8008F04C */
/* @SymbolSize: 840 */
/* Function: SsUtKeyOnV */
/* Address: 0x8008F054 */
/* Size: 832 bytes (208 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 (as1-scheduled, ASPSX div order) */
/* @CompileFlags: -Wc,-pic1 */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_AS1_XBB_SKIP_CAND=all IDO52_AS1_FORCE_BOPT=0 IDO52_AS1_DIV_MACRO=aspsx-mffirst IDO52_FORCE_WEB_RANGE_SPLIT=26 IDO52_FORCE_WEB_NOSPILL=8.0:5,26.0:5 IDO52_FORCE_WEB_COLOR=8:v1 IDO52_NO_CSE_SITE=cvtl:1 IDO52_AS1_TIE_TAKE=36,55,58,296 IDO52_AS1_TIE_REJECT=229,235,290,291,297,302 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: remap_stack_slots --keep-frame --map 20:36 */
#include "types.h"

extern s32 _snd_ev_flag;
typedef struct {
    u8  b[16];
} ProgAtr;

typedef struct {
    u8  b[10];
    u8  pad_0a[12];
    s16 h16;
    u8  pad_18[8];
} ToneAtr;

typedef struct {
    s8  n_voices;
    u8  pad_e5[11];
    u8  f0;
    u8  pad_f1;
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
    u8  ff;
    u8  b00;
    u8  b01;
    u8  b02;
    u8  b03;
    s8  b04;
    u8  pad_05;
    s16 h06;
    s16 h08;
    s16 h0a;
    u8  pad_0c[16];
    ProgAtr *p1c;
    ToneAtr *p20;
} SndState;

extern SndState spuVmMaxVoice;

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
extern void SpuVmDoAllocate();
extern void vmNoiseOn(u8);
extern s16 note2pitch2(u16, u16);
extern void SpuVmKeyOnNow(s32, s32);

s16 SsUtKeyOnV(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
    s16 arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7;
{
    s32 pan;
    ProgAtr *ptr;
    ToneAtr *entry;
    SndEntry *snd;

    if (_snd_ev_flag == 1) {
        return -1;
    }

    _snd_ev_flag = 1;

    if (arg0 < 0 || arg0 >= 24) {
        _snd_ev_flag = 0;
        return -1;
    }

    if (SpuVmVSetUp(arg1, arg2) != 0) {
        _snd_ev_flag = 0;
        return -1;
    }

    spuVmMaxVoice.h06 = 0x20;
    spuVmMaxVoice.f2 = (s8)arg4;
    spuVmMaxVoice.f3 = (s8)arg5;
    spuVmMaxVoice.fc = (u8)arg3;

    if (arg6 == arg7) {
        spuVmMaxVoice.f5 = 0x40;
        spuVmMaxVoice.f4 = (u8)arg6;
    } else if (arg7 < arg6) {
        pan = (arg7 << 6) / arg6;
        spuVmMaxVoice.f5 = (u8)(s8)pan;
        spuVmMaxVoice.f4 = (u8)arg6;
    } else {
        pan = (arg6 << 6) / arg7;
        spuVmMaxVoice.f5 = (u8)(0x7F - (s8)pan);
        spuVmMaxVoice.f4 = (u8)arg7;
    }

    ptr = spuVmMaxVoice.p1c;
    ptr += arg2;
    spuVmMaxVoice.fa = ptr->b[1];
    spuVmMaxVoice.fb = ptr->b[4];
    spuVmMaxVoice.f0 = ptr->b[0];

    entry = spuVmMaxVoice.p20;
    entry += (s16)((spuVmMaxVoice.f7 * 16) + (s8)arg3);

    spuVmMaxVoice.ff = entry->b[0];
    spuVmMaxVoice.h08 = entry->h16;
    spuVmMaxVoice.fd = entry->b[2];
    spuVmMaxVoice.fe = entry->b[3];
    spuVmMaxVoice.b00 = entry->b[4];
    spuVmMaxVoice.b01 = entry->b[5];
    spuVmMaxVoice.b04 = entry->b[1];
    spuVmMaxVoice.b02 = entry->b[6];
    spuVmMaxVoice.b03 = entry->b[7];

    if (spuVmMaxVoice.h08 == 0) {
        _snd_ev_flag = 0;
        return -1;
    }

    spuVmMaxVoice.h0a = arg0;

    snd = _svm_voice;
    snd += arg0;
    snd->field_0a = 0x20;
    snd->field_12 = arg1;
    snd->field_0c = spuVmMaxVoice.f7;
    snd->field_0e = arg2;
    snd->field_00 = spuVmMaxVoice.h08;
    snd->field_02 = 0;
    snd->field_10 = (s8)arg3;
    snd->field_16[1] = 1;
    snd->field_08 = arg4;

    SpuVmDoAllocate(arg4);

    if (spuVmMaxVoice.h08 == 0xFF) {
        vmNoiseOn((u8)arg0);
    } else {
        SpuVmKeyOnNow(1, note2pitch2((u16)arg4, (u16)arg5));
    }

    _snd_ev_flag = 0;
    return arg0;
}
