/* Address: 0x80090BA0 */
/* Size: 96 bytes (24 instructions) */
/* @SymbolStart: 0x80090BA0 */
/* @SymbolSize: 92 */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_AS1_KEEP_ANDI=all IDO52_AS1_NO_PASS1_PROP=all IDO52_FORCE_WEB_COLOR_NOSPLIT=0,6,14,16 IDO52_FORCE_WEB_COLOR=0:v1,6:t0,14:t1,16:t2 IDO52_AS1_TIE_TAKE=62 */
/* @CompileFlags: -Wc,-pic0 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
#include "types.h"

extern void SpuVmSeKeyOn(s16 x, s16 y, u16 w, u16 h, u16 c, u16 d);

void SsVoKeyOn(s32 xy, s32 wh, u16 c, u16 d)
{
    SpuVmSeKeyOn(xy >> 8, xy & 0xFF, wh >> 8, wh & 0xFF, c, d);
}
