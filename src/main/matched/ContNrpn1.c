/* Address: 0x800937B0 */
/* Size: 224 bytes (56 instructions) */
/* Region: Library (0x80086000+) */
/* @SymbolStart: 0x80093780 */
/* @SymbolSize: 272 */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_AS1_NO_FALLTHRU_FILL=occ:3 IDO52_AS1_NO_LA_PULL=occ:0 IDO52_UGEN_TEMP_REG=8:t0 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
#include "types.h"

extern u8 *_ss_score[];
extern u32 _SsMarkCallback[];
extern s32 ReadDeltaValue(s32, s32);

void ContNrpn1(s16 channel, s16 voice, s32 param) {
    u8 *v1;
    u32 callback;

    v1 = _ss_score[channel] + voice * 168;
    param &= 0xFF;

    if (v1[39] == 1) {
        if (v1[16] == 0) {
            v1[40] = (u8)param;
            ((s8 *)v1)[16] = 1;
            goto cmp40;
        }
    }
    if (v1[22] == 30) {
        goto cmp40;
    }
    if (v1[22] == 20) {
        goto cmp40;
    }
    v1[21] = (u8)param;
    v1[42] = v1[42] + 1;

cmp40:
    if (v1[22] == 40) {
        callback = _SsMarkCallback[(channel << 4) + voice];
        if (callback != 0) {
            ((void (*)(void))callback)();
        }
    }

    *(s32 *)(v1 + 136) = ReadDeltaValue(channel, voice);
}
