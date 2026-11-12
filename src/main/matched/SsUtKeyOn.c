/* Function: SsUtKeyOn */
/* Address: 0x8008EB8C */
/* Size: 936 bytes (234 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 (as1-scheduled, ASPSX div order) */
/* @CompileFlags: -Wc,-pic1 */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_AS1_XBB_SKIP_CAND=all IDO52_AS1_FORCE_BOPT=0 IDO52_AS1_DIV_MACRO=aspsx-mffirst IDO52_AS1_TIE_REJECT=18,19,210,237,243 IDO52_AS1_TIE_TAKE=28,31 IDO52_UGEN_TEMP_REG=50:t6 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: patch_stack_complete --old 48 --new 56 */
/* @PostProc: remap_stack_slots --keep-frame --map 20:44,44:52,46:48,48:56,52:60,56:64,60:68,64:72,68:76,72:80 */
/* @PostProc: swap_instructions --offset1 0x2e8 --offset2 0x2f8 */
/* @PostProc: swap_instructions --offset1 0x2ec --offset2 0x2f0 */
/* @PostProc: swap_instructions --offset1 0x2f0 --offset2 0x2f4 */
/* @PostProc: swap_instructions --offset1 0x2f4 --offset2 0x2f8 */
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

extern s32 SpuVmVSetUp(s16, s16);
extern s32 SpuVmAlloc(s32);
extern void SpuVmDoAllocate(void);
extern void vmNoiseOn(u8);
extern s16 note2pitch2(u16, u16);
extern void SpuVmKeyOnNow(s32, s32);
extern void varargs_spill3(s16, u8, u8);

s16 SsUtKeyOn(arg0, arg1, arg2, arg3, arg4, arg5, arg6)
    s16 arg0, arg1, arg2, arg3, arg4, arg5, arg6;
{
    s16 sp_val;
    s16 sp_result;
    s32 pan;
    ProgAtr *ptr;
    ToneAtr *entry;
    SndEntry *snd;

    if (_snd_ev_flag == 1) {
        return -1;
    }

    _snd_ev_flag = 1;

    if (SpuVmVSetUp(arg0, arg1) != 0) {
        _snd_ev_flag = 0;
        return -1;
    }

    spuVmMaxVoice.h06 = 0x20;
    spuVmMaxVoice.f2 = (s8)arg3;
    spuVmMaxVoice.f3 = (s8)arg4;
    spuVmMaxVoice.fc = (u8)arg2;

    if (arg5 == arg6) {
        spuVmMaxVoice.f5 = 0x40;
        spuVmMaxVoice.f4 = (u8)arg5;
    } else if (arg6 < arg5) {
        pan = (arg6 << 6) / arg5;
        spuVmMaxVoice.f5 = (u8)(s8)pan;
        spuVmMaxVoice.f4 = (u8)arg5;
    } else {
        pan = (arg5 << 6) / arg6;
        spuVmMaxVoice.f5 = (u8)(0x7F - (s8)pan);
        spuVmMaxVoice.f4 = (u8)arg6;
    }

    ptr = spuVmMaxVoice.p1c;
    ptr += arg1;
    spuVmMaxVoice.fa = ptr->b[1];
    spuVmMaxVoice.fb = ptr->b[4];
    spuVmMaxVoice.f0 = ptr->b[0];

    sp_val = (s16)((spuVmMaxVoice.f7 * 16) + (s8)arg2);

    entry = spuVmMaxVoice.p20;
    entry += sp_val;

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

    sp_result = SpuVmAlloc(spuVmMaxVoice.h08);

    if (spuVmMaxVoice.n_voices == sp_result) {
        _snd_ev_flag = 0;
        return -1;
    }

    spuVmMaxVoice.h0a = sp_result;

    snd = _svm_voice;
    snd += sp_result;
    snd->field_0a = 0x20;
    snd->field_12 = arg0;
    snd->field_0c = spuVmMaxVoice.f7;
    snd->field_0e = arg1;
    snd->field_00 = spuVmMaxVoice.h08;
    snd->field_10 = spuVmMaxVoice.fc;
    snd->field_02 = 0;
    snd->field_16[1] = 1;
    snd->field_08 = arg3;

    SpuVmDoAllocate();

    if (spuVmMaxVoice.h08 == 0xFF) {
        vmNoiseOn((u8)sp_result);
    } else {
        SpuVmKeyOnNow(1, note2pitch2((u16)arg3, (u16)arg4));
    }

    if (spuVmMaxVoice.b04 & 1) {
        entry = spuVmMaxVoice.p20;
        entry += sp_val;
        varargs_spill3(sp_result, entry->b[8], entry->b[9]);
    }

    _snd_ev_flag = 0;
    return sp_result;
}
