/* Function: SpuVmInit */
/* Address: 0x8008D7BC */
/* Size: 660 bytes (165 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 (native as1) */
/* @CompileFlags: -Wo,-do_opt_saved_regs -cckr */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_CONST_NOINREG_VALUES=24:9:9,16384 IDO52_FORCE_WEB_COLOR=125:s4,126:s1,40:s2,124:s3 IDO52_AS1_XBB_SKIP_CAND=1 IDO52_AS1_TIE_SRC_ORDER=bb:11 IDO52_AS1_TIE_TAKE=225,311,321 IDO52_AS1_TIE_REJECT=9,19,39,48,64,72,96,97,229,312 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern s16 _svm_vcf;
extern s16 _svm_damper;
extern s16 _svm_vab_count;
extern s8  spuVmMaxVoice;
extern s16 D_8013160A;
extern s16 _svm_vg;
extern s16 libsnd_kon_hi_shadow;
extern s16 _svm_okof1;
extern s16 _svm_orev1;
extern s16 _svm_orev2;
extern s32 _svm_rattr;
extern s16 libsnd_reverb_depth_left;
extern s16 libsnd_reverb_depth;
extern s32 libsnd_reverb_type;
extern s8  _svm_auto_kof_mode;
extern s16 _svm_stereo_mono;
extern s16 kMaxPrograms;

extern u16 _svm_sreg_buf[];
extern u8  _svm_sreg_dirty[];
extern u8  _svm_vab_used[];
extern u16 *libsnd_spu_regs_ptr;
extern u8  _ss_spu_vm_rec[];

typedef struct VoiceEntry {
    u16 f00;
    u16 f02;
    u16 f04;
    u16 f06;
    u8  pad08[2];
    s16 f0A;
    s16 f0C;
    s16 f0E;
    u16 f10;
    u8  pad12[5];
    s8  f17;
} VoiceEntry;

typedef struct VolEntry {
    s16 f0;
    s16 f2;
    s16 f4;
    s16 f6;
} VolEntry;

extern VoiceEntry _svm_voice[];
extern VolEntry libsnd_seq_vol_tbl[];

void _spu_setInTransfer();
void SpuInitMalloc();
void SpuVmKeyOffNow();
void SpuVmFlush();

void SpuVmInit(numVoices)
    s8 numVoices;
{
    u16 i;

    _spu_setInTransfer(0);
    _svm_vcf = 0;
    _svm_damper = 0;
    SpuInitMalloc(32, _ss_spu_vm_rec);

    for (i = 0; i < 192; i++) {
        _svm_sreg_buf[i] = 0;
    }
    for (i = 0; i < 24; i++) {
        _svm_sreg_dirty[i] = 0;
    }
    _svm_vab_count = 0;
    for (i = 0; i < 16; i++) {
        _svm_vab_used[i] = 0;
    }

    if ((u16)numVoices >= 24) {
        spuVmMaxVoice = 24;
    } else {
        spuVmMaxVoice = (u16)numVoices;
    }

    i = 0;
    if (spuVmMaxVoice != 0) {
        u16 *p;
        u16 *se;

        p = libsnd_spu_regs_ptr;
        do {
            _svm_voice[i].f02 = 24;
            _svm_voice[i].f00 = 255;
            _svm_voice[i].f17 = 0;
            _svm_voice[i].f04 = 0;
            _svm_voice[i].f06 = 0;
            _svm_voice[i].f0A = -1;
            _svm_voice[i].f0C = 0;
            _svm_voice[i].f0E = 0;
            _svm_voice[i].f10 = 255;
            se = (u16 *)((u8 *)p + (u16)(i * 8) * 2);
            se[0] = 0;
            se[1] = 0;
            se[3] = 512;
            se[5] = 16384;
            se[2] = 4096;
            se[4] = 0x80ff;
            D_8013160A = i;
            SpuVmKeyOffNow(1);
            i++;
        } while (i < spuVmMaxVoice);
    }

    for (i = 0; i < 33; i++) {
        libsnd_seq_vol_tbl[i].f0 = 127;
        libsnd_seq_vol_tbl[i].f2 = 127;
        libsnd_seq_vol_tbl[i].f4 = 127;
        libsnd_seq_vol_tbl[i].f6 = 64;
    }

    _svm_vg = 0;
    libsnd_kon_hi_shadow = 0;
    _svm_okof1 = 0;
    _svm_orev1 = 0;
    _svm_orev2 = 0;
    _svm_rattr = 0;
    libsnd_reverb_depth_left = 16383;
    libsnd_reverb_depth = 16383;
    libsnd_reverb_type = 0;
    _svm_auto_kof_mode = 0;
    _svm_stereo_mono = 0;
    kMaxPrograms = 128;
    SpuVmFlush();
}
