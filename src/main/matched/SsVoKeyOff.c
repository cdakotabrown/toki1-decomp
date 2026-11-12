/* Address: 0x80090B58 (recorded symbol = prologue; real entry 0x80090B50) */
/* Size: 80 bytes (20 words: 19 code + 1 trailing alignment nop) */
/* Region: Library (IDO 5.2 as1 + convert_or_to_addu) */
/* @SymbolStart: 0x80090B50 */
/* @SymbolSize: 76 */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_FORCE_WEB_COLOR_NOSPLIT=0,6 IDO52_FORCE_WEB_COLOR=0:v1,6:t0 IDO52_AS1_KEEP_ANDI=all IDO52_AS1_NO_PASS1_PROP=all */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
#include "types.h"

extern void SpuVmSeKeyOff();

void SsVoKeyOff(xy, wh)
s32 xy, wh;
{
    SpuVmSeKeyOff((s16)(xy >> 8), (s16)(xy & 0xFF), (u16)(wh >> 8), (u16)(wh & 0xFF));
}
