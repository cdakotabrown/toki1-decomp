/* Address: 0x8008FBD0 */
/* Size: 164 bytes (41 instructions) */
/* Region: Library (0x80086000+) */
/* @SymbolStart: 0x8008FBD0 */
/* @SymbolSize: 164 */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_AS1_NO_COPY_PROP=3 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern s32 _snd_openflag;
extern char libsnd_seq_open_limit_msg;

void printf();
void InitSoundSeq();

s16 SsSeqOpen(s32 arg0, s16 arg1) {
    s32 i;
    s32 found;
    s16 unused;
    s16 bit;

    if (_snd_openflag == -1) {
        printf(&libsnd_seq_open_limit_msg);
        return -1;
    }
    i = 0;
    found = 0;
    do {
        if (!(_snd_openflag & (1U << i))) {
            bit = i;
            found = 1;
        }
        i++;
    } while (found == 0);
    _snd_openflag |= 1 << bit;
    InitSoundSeq(bit, arg1, arg0);
    return bit;
}
