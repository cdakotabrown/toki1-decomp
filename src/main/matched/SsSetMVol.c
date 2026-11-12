/* Address: 0x8008B290 */
/* Size: 116 bytes (29 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 -> needs -Wc,-pic0 and */
/* @SymbolStart: 0x8008B290 */
/* @SymbolSize: 116 */
/* @CompileFlags: -Wc,-pic0 */
/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=127,16383 IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_FORCE_WEB_COLOR=5:v1,12:a3 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
#include "types.h"

extern void _spu_ioctl(s32 arg0, s32 arg1, void *arg2);

typedef struct {
    s32 cmd;
    s16 x;
    s16 y;
    s8  pad[44];
} SndPosPacket;

void SsSetMVol(s16 arg0, s16 arg1) {
    SndPosPacket pkt;

    pkt.cmd = 3;
    pkt.x = (arg0 * 16383) / 127;
    pkt.y = (arg1 * 16383) / 127;
    _spu_ioctl(0, 256, &pkt);
}
