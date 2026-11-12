/* Function: SpuIsTransferCompleted */
/* Address: 0x80095510 */
/* Size: 188 bytes (47 instructions) */
/* Region: Library (0x80086000+), compiler IDO 5.2 (no `j`, t6/t7/t8/t9 */
/* @SymbolStart: 0x80095510 */
/* @SymbolSize: 188 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_stack_complete --old 24 --new 32 --shift-ra */
/* @CompileEnv: IDO52_AS1_NO_LA_PULL=all */
/* @CompileEnv: IDO52_AS1_NO_FALLTHRU_FILL=all */
#include "types.h"

extern s32 _spu_trans_mode;
extern s32 _spu_inTransfer;
extern u32 _spu_EVdma;
extern s32 TestEvent(u32 arg0);

s32 SpuIsTransferCompleted(s32 arg0) {
    s32 r;

    if (_spu_trans_mode == 1) {
        return 1;
    }
    if (_spu_inTransfer == 1) {
        return 1;
    }
    r = TestEvent(_spu_EVdma);
    if (arg0 == 1) {
        while (r == 0) {
            r = TestEvent(_spu_EVdma);
        }
        _spu_inTransfer = 1;
        return r;
    }
    if (r == 1) {
        _spu_inTransfer = 1;
    }
    return r;
}
