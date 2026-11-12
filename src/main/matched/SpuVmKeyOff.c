/* Address: 0x8008E420 */
/* Size: 316 bytes (79 instructions) */
/* Region: Library (0x80086000+) */
/* @CompileFlags: -Wo,-do_opt_saved_regs -cckr */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_CONST_NOINREG_VALUES=24 IDO52_AS1_XBB_SKIP_CAND=all IDO52_FORCE_WEB_COLOR=16:s4,23:s5,29:s6,35:s7 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: swap_instructions --offset1 0x4 --offset2 0xc */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0x10 */
/* @PostProc: swap_instructions --offset1 0x10 --offset2 0x14 */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x24 */
/* @PostProc: swap_instructions --offset1 0x1c --offset2 0x24 */
/* @PostProc: swap_instructions --offset1 0x20 --offset2 0x24 */
/* @PostProc: swap_instructions --offset1 0x30 --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x34 --offset2 0x4c */
/* @PostProc: swap_instructions --offset1 0x38 --offset2 0x4c */
/* @PostProc: swap_instructions --offset1 0x3c --offset2 0x4c */
/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x4c */
#include "types.h"

typedef struct {
    s16 kind;
    s16 pad2;
    s16 pad4;
    s16 pad6;
    s16 f8;
    s16 fa;
    s16 pad12;
    s16 fe;
    s16 pad16;
    s16 f12;
    s16 pad20;
    s16 pad22;
} Rec24;
extern Rec24 _svm_voice[];
extern s8 spuVmMaxVoice;
extern s16 D_8013160A;
extern void vmNoiseOff();
extern void SpuVmKeyOffNow();

SpuVmKeyOff(a, b, c, d)
    short a, b, c;
    unsigned short d;
{
    unsigned char i;
    unsigned char n;
    Rec24 *r;

    i = 0;
    n = 0;
    for (; i < spuVmMaxVoice; i++) {
        r = &_svm_voice[i];
        if (d == r->f8 && c == r->fe && a == r->fa && b == r->f12) {
            if (r->kind == 255) {
                vmNoiseOff(i);
            } else {
                D_8013160A = i;
                SpuVmKeyOffNow(0);
            }
            n++;
        }
    }
    return n;
}
