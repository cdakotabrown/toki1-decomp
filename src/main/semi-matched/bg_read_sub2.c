/* Address: 0x8008D430 */
/* Size: 280 bytes (70 instructions) */
/* Region: Library (0x80086000+) */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_AS1_KEEP_ANDI=all */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count auto */
/* @Measured: 2026-09-17 16/70 exact (22.9%), compiled 70 vs target 70 words, Fixup 0 -- SEMI */
#include "types.h"

extern u8  _svm_sreg_dirty[];
extern u16 _svm_sreg_buf[];
extern u8  _svm_voice[];
extern u8 * volatile libsnd_spu_regs_ptr;
extern u16 _svm_vg;
extern u16 libsnd_kon_hi_shadow;
extern u16 _svm_okof1;
extern u16 _svm_okof2;

void bg_read_sub2(u32 ch, u32 pa, u32 pb, s32 unused) {
    u8  *status;
    u16 *pp;
    u16  idx;
    u16  onLo;
    u16  onHi;
    u8  *ctrl;
    u8  *st;
    u16  newLo;
    u16  newHi;

    ch &= 0xFF;
    idx = ch * 8;
    pp = &_svm_sreg_buf[idx];
    status = &_svm_sreg_dirty[ch];
    pa &= 0xFFFF;
    pb &= 0xFFFF;
    *status |= 3;
    pp[1] = pb;
    pp[0] = pa;
    if (ch < 16) {
        onLo = 1 << ch;
        onHi = 0;
    } else {
        onHi = 1 << (ch + 16);
        onLo = 0;
    }
    ctrl = &_svm_voice[ch * 24];
    st = libsnd_spu_regs_ptr;
    *(u16 *)(ctrl + 4) = 10;
    ctrl[23] = 1;
    *(u16 *)(ctrl + 2) = 0;
    newLo = _svm_vg | onLo;
    _svm_vg = newLo;
    newHi = libsnd_kon_hi_shadow | onHi;
    libsnd_kon_hi_shadow = newHi;
    _svm_okof1 &= ~newLo;
    _svm_okof2 &= ~newHi;
    *(u16 *)(st + 406) = onHi;
    *(u16 *)(st + 404) = onLo;
}
