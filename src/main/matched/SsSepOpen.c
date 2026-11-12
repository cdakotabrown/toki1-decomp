/* Address: 0x8008FC74 */
/* Size: 248 bytes (62 instructions) -- the word to 0x8008FD70 is an alignment nop */
/* @SymbolSize: 248 */
/* @CompileFlags: -Wc,-pic0 -Wo,-do_opt_saved_regs */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* - `@CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all` (Hook 41b): the s16 */
#include "types.h"

extern volatile s32 _snd_openflag;
extern u8 libsnd_sep_open_limit_msg[];

extern void printf(u8 *msg);
extern s32 InitSoundSep(s32 slot, s32 k, s32 w, u8 *p);

s32 SsSepOpen(u8 *p0, s16 w, s16 n)
{
    s32 mask;
    s32 bit;
    s32 i;
    s32 found;
    s16 k;
    s16 slot;
    u8 *p;

    mask = _snd_openflag;
    p = p0;
    if (mask == -1) {
        printf(libsnd_sep_open_limit_msg);
        return -1;
    }

    i = 0;
    found = 0;
    do {
        bit = 1 << i;
        if ((mask & bit) == 0) {
            slot = (s16)i;
            found = 1;
        }
        i++;
    } while (found == 0);

    mask |= 1 << slot;
    _snd_openflag = mask;

    for (k = 0; k < n; k++) {
        p += InitSoundSep(slot, k, w, p);
    }
    return slot;
}
