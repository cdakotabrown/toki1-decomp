/* Address: 0x8008CE94 */
/* Size: 504 bytes (126 instructions) */
/* `@Compiler: ido52-gas` on the final C (as1 hooks dropped, no */
/* @CompileFlags: -Wc,-pic0 */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=52:t9,53:t1 IDO52_AS1_TIE_TAKE=185,186,207,208,228,230 IDO52_AS1_TIE_REJECT=113,114,130,133,138,139,159,160,161,187,209 IDO52_AS1_DIV_POW2=aspsx IDO52_FORCE_WEB_COLOR=25:a2 IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_COPYPROP_SITE=lod:2:deny,lod:7:deny,lod:10:deny,lod:13:deny IDO52_AS1_KEEP_DEAD_RELOAD=0 IDO52_AS1_NO_COPY_PROP=0 IDO52_AS1_NO_LA_PULL=all IDO52_AS1_NO_FALLTHRU_FILL=all */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: patch_insn_reg_field 0x1e4:rd:a2:t5 0x1f4:rt:a2:t5 */
#include "types.h"

typedef struct {
    u8  pad[6];
    s16 f6;
    u8  pad2[16];
} V24;

typedef struct {
    u8 pad[12];
    u16 f12;
    u16 f14;
} VagEnt;

typedef struct {
    u8  p0[3];
    s8  bank;
    u8  p1[4];
    s8  prog;
    u8  p2[11];
    s16 tone;
    s16 voice;
    s16 voff;
    s16 slot;
    u16 pan;
    u8  p3[10];
    VagEnt *vag;
    u8 (*tbl)[32];
} SndState;
extern SndState D_801315F4;
extern V24 _svm_voice[];
extern u32 _svm_envx_hist[];
extern s16 _svm_sreg_buf[];
extern u8  _svm_sreg_dirty[];

void SpuVmDoAllocate(void)
{
    s16 voice;
    s32 vsh;
    u32 mask;
    s32 i;
    s16 *vp;
    u8 *fp;
    u8 *ap;
    u16 pan;

    voice = D_801315F4.voice;
    D_801315F4.voff = voice * 8;
    vsh = (s8)D_801315F4.bank << 4;
    D_801315F4.slot = (s32)vsh + (s32)(s8)D_801315F4.prog;

    mask = ~(1 << voice);
    _svm_voice[voice].f6 = 32767;

    i = 0;
    while (i < 16) {
        _svm_envx_hist[i] &= mask;
        i++;
    }

    if ((D_801315F4.tone & 1) > 0) {
        vp = &_svm_sreg_buf[D_801315F4.voff];
        vp[3] = D_801315F4.vag[(D_801315F4.tone - 1) / 2].f12;
        fp = &_svm_sreg_dirty[D_801315F4.voice];
        *fp |= 8;
    } else {
        vp = &_svm_sreg_buf[D_801315F4.voff];
        vp[3] = D_801315F4.vag[(D_801315F4.tone - 1) / 2].f14;
        fp = &_svm_sreg_dirty[D_801315F4.voice];
        *fp |= 8;
    }

    ap = (u8 *)&D_801315F4.tbl[vsh][(s8)D_801315F4.prog << 5];
    vp[4] = *(u16 *)(ap + 16);
    pan = *(u16 *)(ap + 18) + D_801315F4.pan;
    *fp |= 0x30;
    vp[5] = pan;
}
