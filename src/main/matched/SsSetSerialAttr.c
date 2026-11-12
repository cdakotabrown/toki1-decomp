/* Address: 0x8008BEE8 (recorded); TRUE START 0x8008BEE0, 168 bytes / 42 words */
/* Region: Library (0x80086000+), IDO 5.2 -O2 */
/* @SymbolStart: 0x8008BEE0 */
/* @SymbolSize: 168 */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_AS1_NO_FALLTHRU_FILL=all IDO52_AS1_NO_LA_PULL=all */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
#include "types.h"

extern void _spu_ioctl(s32, s32, void *);

void SsSetSerialAttr(s8 arg0, s8 arg1, s8 arg2) {
    s32 params[13];
    s32 v0;

    if (arg0 != 0) {
        v0 = 1;
    } else {
        v0 = 1;
        if (arg1 == 0) {
            params[0] = 1024;
            params[5] = arg2;
        }
        v0 = 1;
        if (arg1 == v0) {
            params[0] = 256;
            params[4] = arg2;
        }
    }

    if (arg0 != v0) {
    } else {
        if (arg1 == 0) {
            params[0] = 2048;
            params[8] = arg2;
        }
        if (arg1 != v0) {
        } else {
            params[0] = 512;
            params[7] = arg2;
        }
    }

    _spu_ioctl(0, 256, params);
}
