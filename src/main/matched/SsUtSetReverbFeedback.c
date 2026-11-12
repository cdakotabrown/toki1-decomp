/* Function: SsUtSetReverbFeedback */
/* Address: 0x8008FB50  (registered address corrected 2026-09-17; see below) */
/* Size: 64 bytes (16 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @SymbolStart: 0x8008FB50 */
/* @SymbolSize: 64 */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: patch_insn_reg_field 0x0:rd:a1:v1 0x18:rt:a1:v1 0x18:rd:a1:v1 0x24:rt:a1:v1 */
#include "types.h"

extern s32 _svm_rattr;
extern s32 libsnd_reverb_feedback;

extern void SpuSetReverbModeParam(s32 *arg);

void SsUtSetReverbFeedback(s16 arg0) {
    _svm_rattr = 16;
    libsnd_reverb_feedback = arg0;
    SpuSetReverbModeParam(&_svm_rattr);
}
