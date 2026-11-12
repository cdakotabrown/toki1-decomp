/* Address: 0x8008C7A0 (recorded symbol = mid-head; true entry 0x8008C774) */
/* Size: 128 bytes (32 instructions) from the true entry */
/* Region: Library (0x80086000+) */
/* @SymbolStart: 0x8008C774 */
/* @SymbolSize: 128 */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=1,2,4 IDO52_UGEN_NO_TARGET=5,7 IDO52_UGEN_TEMP_REG=1:t2,2:t2,3:t3,4:t3 IDO52_AS1_KEEP_SEXT=all IDO52_AS1_NO_PASS1_PROP=26,27,28,29 IDO52_AS1_NO_COPY_PROP=all */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR_NOSPLIT=7,12,15 IDO52_FORCE_WEB_COLOR=7:t1,12:v1,15:t0 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u8  pad0[114];
    s16 x;
    s16 y;
    u8  pad76[50];
} Chan168;

extern Chan168 *_ss_score[];
extern void SpuVmSetSeqVol(s16 bank, u16 x, u16 y, s32 z);

void Snd_SetVol(s16 bank, s16 ch, s16 x, s16 y)
{
    Chan168 *c = &_ss_score[bank][ch];

    c->x = x;
    c->y = y;
    SpuVmSetSeqVol(bank, x, y, 0);
}
