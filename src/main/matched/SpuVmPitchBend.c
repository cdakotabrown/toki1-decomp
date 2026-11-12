/* Function: SpuVmPitchBend */
/* Address: 0x8008DB90 */
/* Size: 560 bytes (140 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @CompileFlags: -Wo,-do_opt_saved_regs -cckr */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_AS1_NO_SHIFT_COMBINE=all IDO52_CONST_NOINREG_VALUES=63,24 IDO52_AS1_KEEP_SEXT=all IDO52_COPYPROP_VAR=-24 IDO52_AS1_NO_FALLTHRU_FILL=all IDO52_AS1_NO_LA_PULL=all IDO52_AS1_TIE_TAKE=13,33 IDO52_AS1_TIE_REJECT=126 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_insn_reg_field 0x118:rs:t1:v1 0x118:rt:v1:t1 0x168:rs:t6:v1 0x168:rt:v1:t6 */






































































































#include "types.h"

typedef struct {
    u8 pad0[8];
    u16 velocity;
    s16 note;
    u8 pad1[2];
    s16 keyRange;
    s16 pitch;
    s16 program;
    u8 pad2[4];
} SndEntry;

extern SndEntry _svm_voice[];
extern s16 _svm_sreg_buf[];
extern u8 _svm_sreg_dirty[];
extern s8 spuVmMaxVoice;
extern s16 D_80131606;
extern s16 D_8013160A;
extern s8 libsnd_cur_fake_program;
extern u8 *_svm_tn;
extern s8 libsnd_cur_tone_idx;

extern void SpuVmVSetUp(s32, s32);
extern s16 note2pitch2();

#line 1000
s32 SpuVmPitchBend(s16 note, s16 prog, s16 key, u16 vmod) {
    s32 cnt;
    s8 i;
    SndEntry *base;
    SndEntry *e;
    s16 pitch;
    u16 vel;
    s16 dv;
    u16 idx;
    u32 prod;
    u16 hi;
    u16 lo;
    s16 *r;

#line 1000
    cnt = 0; SpuVmVSetUp(prog, key);
    D_80131606 = (s8)note & 0xFF;
    i = 0;
    if (spuVmMaxVoice > 0) {
        base = _svm_voice;
        do {
            e = base;
            e += i;
            if (note == e->note && prog == e->program && key == e->keyRange) {
                pitch = e->pitch;
                dv = vmod - 64;
                vel = e->velocity;
                idx = (libsnd_cur_fake_program << 4) + pitch;
                if (dv > 0) {
                    prod = *(_svm_tn + 13 + idx * 32) * dv;
                    hi = vel + prod / 63; lo = (prod % 63) << 1;
                } else if (dv < 0) {
                    prod = *(_svm_tn + 12 + idx * 32) * dv;
                    hi = vel + (prod >> 6) - 1; lo = ((prod & 0x3F) << 1) + 127;
                } else {
                    hi = vel;
                    lo = 0;
                }
                libsnd_cur_tone_idx = pitch;
                D_8013160A = i;
                r = _svm_sreg_buf;
                r += i * 8;
                r[2] = note2pitch2(hi, lo);
                _svm_sreg_dirty[i] |= 4;
                cnt++;
            }
            i++;
        } while (i < spuVmMaxVoice);
    }
    return cnt;
}
