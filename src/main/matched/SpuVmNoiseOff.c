/* Function: SpuVmNoiseOff */
/* Address: 0x8008DAD8   (true entry 0x8008DAD0 -- see WINDOW below) */
/* Size: 148 bytes (37 instructions) */
/* Region: Library (0x80086000+) */
/* Compiler: IDO 5.2 (t6/t7/t8/t9 scratch, no `j`, `or`-form move) */
/* @SymbolStart: 0x8008DAD0 */
/* @SymbolSize: 148 */
/* @CompileFlags: -Wo,-do_opt_saved_regs */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_UGEN_TEMP_REG=3:t7,4:t7,7:t0 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: swap_instructions --offset1 0x10 --offset2 0x18 */
#include "types.h"

extern s8 spuVmMaxVoice;
extern s8 _svm_voice[];

extern void vmNoiseOff(u8);

void SpuVmNoiseOff(void) {
    s16 i;

    for (i = 0; i < spuVmMaxVoice; i++) {
        if (_svm_voice[(((i << 2) - i) << 3) + 23] == 1) {
            vmNoiseOff((u8)i);
        }
    }
}
