/* Address: 0x8008FAD4 */
/* Size: 124 bytes (31 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 -O2 */
/* @SymbolStart: 0x8008FAD4 */
/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=127,32767 */
/* @SymbolSize: 124 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_insn_reg_field --patch 0x0:rd:a2:v1 --patch 0x4:rt:a2:v1 --patch 0x4:rd:t6:v1 --patch 0x8:rt:t9:t6 --patch 0x10:rt:t9:t6 --patch 0x14:rt:t6:v1 --patch 0x14:rd:t0:t7 --patch 0x18:rs:t0:t7 --patch 0x18:rt:t6:v1 --patch 0x18:rd:t0:t7 --patch 0x20:rs:t0:t7 --patch 0x24:rd:t7:a1 --patch 0x28:rt:t7:a1 --patch 0x28:rd:t8:a1 --patch 0x30:rt:t8:a1 --patch 0x30:rd:t2:t9 --patch 0x34:rs:t2:t9 --patch 0x34:rt:t8:a1 --patch 0x34:rd:t2:t9 --patch 0x48:rd:t1:t8 --patch 0x4C:rt:t1:t8 --patch 0x54:rs:t2:t9 --patch 0x5C:rd:t3:t0 --patch 0x60:rt:t3:t0 */
#include "types.h"

extern s32 _svm_rattr;
extern s16 libsnd_reverb_depth_left;
extern s16 libsnd_reverb_depth;
extern void SpuSetReverbModeParam(s32 *rec);

void SsUtSetReverbDepth(s16 a0, s16 a1)
{
    _svm_rattr = 6;
    libsnd_reverb_depth_left = (a0 * 32767) / 127;
    libsnd_reverb_depth = (a1 * 32767) / 127;
    SpuSetReverbModeParam(&_svm_rattr);
}
