/* Address: 0x8008D430 */
/* Size: 280 bytes (70 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 -O2 */
/* @CompileFlags: -Wc,-pic0 -cckr */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_AS1_KEEP_ANDI=all IDO52_AS1_TIE_TAKE=30,33 IDO52_AS1_TIE_REJECT=17,34 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: swap_instructions --offset1 0x2c --offset2 0x34 */
/* @PostProc: swap_instructions --offset1 0x30 --offset2 0x38 */
/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x84 --offset2 0x88 */
/* @PostProc: swap_instructions --offset1 0x88 --offset2 0x8c */
/* @PostProc: swap_instructions --offset1 0xa0 --offset2 0xa4 */
#include "types.h"

typedef struct {
    u8  pad[2];
    s16 f2;
    s16 f4;
    u8  pad2[17];
    u8  f23;
} V24b;

extern u8  _svm_sreg_dirty[];
extern s16 _svm_sreg_buf[];
extern V24b _svm_voice[];
extern u8 * volatile libsnd_spu_regs_ptr;
extern u16 _svm_vg;
extern u16 libsnd_kon_hi_shadow;
extern u16 _svm_okof1;
extern u16 _svm_okof2;

void vmNoiseOn2(voice, left, right, arg3)
u8 voice;
u16 left;
u16 right;
s16 arg3;
{
    u16 lo;
    u16 hi;
    u8 *g;
    u8 *fp;
    s16 *vp;
    V24b *e;

    vp = &_svm_sreg_buf[(u16)(voice * 8)];
    fp = &_svm_sreg_dirty[voice];
    vp[1] = right;
    *fp |= 3;
    vp[0] = left;
    if (voice < 16) {
        lo = 1 << voice;
        hi = 0;
    } else {
        hi = 1 << (voice + 16);
        lo = 0;
    }
    e = &_svm_voice[voice];
    g = libsnd_spu_regs_ptr;
    e->f4 = 10;
    e->f23 = 1;
    e->f2 = 0;
    _svm_vg |= lo;
    libsnd_kon_hi_shadow |= hi;
    _svm_okof1 &= ~_svm_vg;
    _svm_okof2 &= ~libsnd_kon_hi_shadow;
    *(u16 *)(g + 406) = hi;
    *(u16 *)(g + 404) = lo;
}
