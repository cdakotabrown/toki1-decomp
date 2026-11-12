/* Function: _spu_close */
/* Address: 0x800959DC */
/* Size: 52 bytes (13 instructions) */
/* Region: IDO (Library, >= 0x80086000) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: scheduler_normalizer */
#include "types.h"

void *_SpuDataCallback(s32);
extern s32 _spu_transferCallback;
extern s32 _spu_IRQCallback;

s32 _spu_close(s32 arg0) {
    _spu_transferCallback = 0;
    _spu_IRQCallback = 0;
    _SpuDataCallback(0);
    return 1;
}
