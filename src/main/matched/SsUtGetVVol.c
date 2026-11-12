/* Function: SsUtGetVVol */
/* Address: 0x8008F70C */
/* Size: 132 bytes (33 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @SymbolStart: 0x8008F70C */
/* @SymbolSize: 132 */
/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=127,16383 IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_AS1_NO_FALLTHRU_FILL=1 IDO52_AS1_NO_SHIFT_COMBINE=all IDO52_AS1_NO_LA_PULL=all IDO52_FORCE_WEB_COLOR=2:a3,7:t0,26:a1 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
#include "types.h"

extern s16 *libsnd_spu_regs_ptr;

s32 SsUtGetVVol(s16 vc, s16 *outL, s16 *outR)
{
    s16 *r;
    s16 *pl = outL;
    s16 *pr = outR;
    s32 idx;
    s32 l;
    s32 rr;

    if (vc < 24) {
        if (vc >= 0) {
            idx = vc * 8;
            r = libsnd_spu_regs_ptr;
            r += idx;
            l = r[0];
            rr = r[1];
            *pl = (s16)((l * 127) / 16383);
            *pr = (s16)((rr * 127) / 16383);
            return 0;
        }
    }
    return -1;
}
