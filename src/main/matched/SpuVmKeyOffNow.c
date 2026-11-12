/* Address: 0x8008CD18 */
/* Size: 200 bytes (50 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @CompileFlags: -Wc,-pic0 */
/* @CompileEnv: IDO52_AS1_KEEP_ANDI=occ:2,3 IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_AS1_NO_LA_PULL=all IDO52_AS1_NO_FALLTHRU_FILL=1 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x48 --offset2 0x4c */
/* @PostProc: swap_instructions --offset1 0x44 --offset2 0x48 */
/* @PostProc: swap_instructions --offset1 0x4c --offset2 0x50 */
/* @PostProc: swap_instructions --offset1 0x54 --offset2 0x58 */
/* @PostProc: swap_instructions --offset1 0x58 --offset2 0x5c */
#include "types.h"

extern u16 D_8013160A;
extern u16 _svm_vg;
extern u16 libsnd_kon_hi_shadow;
extern volatile u16 _svm_okof1;
extern volatile u16 _svm_okof2;

extern u8  _svm_voice[];

void SpuVmKeyOffNow(s32 a0) {
    u32 idx;
    u32 idx_hi;
    u32 lo_mask;
    u32 hi_mask;
    u32 set_lo;
    u32 set_hi;
    u8 *entry;

    (void)a0;

    idx = D_8013160A;
    idx_hi = idx + 16;
    if (idx < 16) {
        lo_mask = (u16)(1 << idx);
        hi_mask = 0;
    } else {
        hi_mask = (u16)(1 << idx_hi);
        lo_mask = 0;
    }

    entry = _svm_voice + idx * 24;
    set_lo = _svm_okof1;
    set_hi = _svm_okof2;
    *(u16 *)(entry + 4) = 0;
    entry[23] = 0;
    *(u16 *)(entry + 0) = 0;

    set_lo |= lo_mask;
    set_lo &= 0xFFFF;
    set_hi |= hi_mask;
    set_hi &= 0xFFFF;
    _svm_vg = _svm_vg & ~set_lo;
    libsnd_kon_hi_shadow = libsnd_kon_hi_shadow & ~set_hi;
    _svm_okof1 = set_lo;
    _svm_okof2 = set_hi;
}
