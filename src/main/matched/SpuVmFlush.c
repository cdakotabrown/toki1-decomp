/* Address: 0x8008DDC0 */
/* Size: 804 bytes (201 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 -O2 (as1-scheduled: the lb/lui */
/* @CompileEnv: IDO52_NO_LOOP_TEST_REPLACEMENT=bb:16 IDO52_FORM_SPLIT=lda:0x0:145 IDO52_CONST_NOINREG_VALUES=24:1:1 IDO52_AS1_NO_LA_PULL=all IDO52_AS1_NO_FALLTHRU_FILL=all IDO52_AS1_KEEP_DEAD_RELOAD=0,1 IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_COPYPROP_SITE=lod:4:deny IDO52_FORCE_WEB_COLOR=24:v1,15:a2,113:a2,115:a1,0:a1,59:a0,10:v1,12:a0 IDO52_AS1_TIE_REJECT=48,239 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
#include "types.h"

extern s32 _svm_envx_ptr;
extern u32 _svm_envx_hist[16];
extern s8 spuVmMaxVoice;
extern volatile u16 * volatile libsnd_spu_regs_ptr;
extern u8 _svm_voice[];
extern s8 _svm_auto_kof_mode;
extern u16 _svm_vg;
extern u16 libsnd_kon_hi_shadow;
extern u16 _svm_okof1;
extern u16 _svm_okof2;
extern u16 _svm_orev1;
extern u16 _svm_orev2;
extern u8 _svm_sreg_dirty[24];
extern u16 _svm_sreg_buf[];

void SpuVmFlush(void)
{
    volatile u16 *spu;
    u32 *slot;
    s32 mask;
    s32 i;
    s32 j;
    s32 k;
    u8 *rec;
    volatile u16 *vreg;
    u32 flags;
    s32 n;
    s32 m;
    s32 cnt;

    cnt = spuVmMaxVoice;
    n = (_svm_envx_ptr + 1) & 0xF;
    slot = &_svm_envx_hist[n];
    *slot = 0;
    _svm_envx_ptr = n;

    i = 0;
    if (cnt <= 0) {
        spu = libsnd_spu_regs_ptr;
    } else {
        spu = libsnd_spu_regs_ptr;
        rec = _svm_voice;
        vreg = spu;
        do {
            *(volatile u16 *)(rec + 6) = vreg[6];
            if (*(volatile u16 *)(rec + 6) != 0) {
                cnt = spuVmMaxVoice;
                i++;
            } else {
                cnt = spuVmMaxVoice;
                *slot |= 1 << i;
                i++;
            }
            rec += 24;
            vreg += 8;
        } while (i < cnt);
    }

    if (_svm_auto_kof_mode == 0) {
        mask = -1;
        for (k = 0; k < 15; k++) {
            mask &= _svm_envx_hist[k];
        }
        j = 0;
        if (cnt > 0) {
            do {
                if (mask & (1U << j)) {
                    _svm_voice[j * 24 + 23] = 0;
                }
                j++;
            } while (j < cnt);
        }
    }

    _svm_vg &= ~_svm_okof1;
    libsnd_kon_hi_shadow &= ~_svm_okof2;

    for (m = 0; m < 24; m++) {
        flags = _svm_sreg_dirty[m];
        if (flags & 1) {
            spu[(u32)(m * 8) + 0] = _svm_sreg_buf[m * 8 + 0];
            spu[(u32)(m * 8) + 1] = _svm_sreg_buf[m * 8 + 1];
            flags = _svm_sreg_dirty[m];
        }
        if (flags & 4) {
            spu[(u32)(m * 8) + 2] = _svm_sreg_buf[m * 8 + 2];
            flags = _svm_sreg_dirty[m];
        }
        if (flags & 8) {
            spu[(u32)(m * 8) + 3] = _svm_sreg_buf[m * 8 + 3];
            flags = _svm_sreg_dirty[m];
        }
        if (flags & 0x10) {
            spu[(u32)(m * 8) + 4] = _svm_sreg_buf[m * 8 + 4];
            spu[(u32)(m * 8) + 5] = _svm_sreg_buf[m * 8 + 5];
        }
        _svm_sreg_dirty[m] = 0;
    }

    spu[0xC6] = _svm_okof1;
    spu[0xC7] = _svm_okof2;
    spu[0xC4] = _svm_vg;
    spu[0xC5] = libsnd_kon_hi_shadow;
    spu[0xCC] = _svm_orev1;
    spu[0xCD] = _svm_orev2;
    _svm_okof1 = 0;
    _svm_okof2 = 0;
    _svm_vg = 0;
    libsnd_kon_hi_shadow = 0;
}
