/* Address: 0x8008BF98 (recorded); TRUE START 0x8008BF90, 256 bytes / 64 words */
/* Region: Library (0x80086000+), IDO 5.2 -O2 */
/* @SymbolStart: 0x8008BF90 */
/* @SymbolSize: 256 */
/* @CompileEnv: IDO52_AS1_NO_FALLTHRU_FILL=all IDO52_AS1_NO_LA_PULL=all IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_CONST_NOINREG_VALUES=127 */
/* @CompileEnv: IDO52_UGEN_TEMP_REG_INPLACE=9 IDO52_UGEN_TEMP_REG=3:t7,6:t9,9:v0,11:t2,12:t2 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern void _spu_ioctl(s32, s32, void *);

void SsSetSerialVol(s8 arg0, s16 arg1, s16 arg2) {
    s32 sp[13];

    if (arg0 == 0) {
        sp[0] = 48;

        if (arg1 >= 128) {
            arg1 = 127;
        }
        if (arg2 >= 128) {
            arg2 = 127;
        }

        *(s16 *)&sp[3] = (s16)(((arg1 << 15) - arg1) / 127);
        *(s16 *)((u8 *)&sp[3] + 2) = (s16)(((arg2 << 15) - arg2) / 127);
    }

    if (arg0 == 1) {
        sp[0] = 192;

        if (arg1 >= 128) {
            arg1 = 127;
        }

        if (arg2 >= 128) {
            arg2 = 127;
        }

        *(s16 *)&sp[6] = (s16)(((arg1 << 15) - arg1) / 127);
        *(s16 *)((u8 *)&sp[6] + 2) = (s16)(((arg2 << 15) - arg2) / 127);
    }

    _spu_ioctl(0, 256, sp);
}
