/* Function: SsSepOpen */
/* Address: 0x8008FC74 */
/* Size: 248 bytes (62 instructions) */
/* Region: Library (0x80086000+) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: custom_script custom_8008FC74.py */
#include "types.h"

extern s32 _snd_openflag;
extern char libsnd_sep_open_limit_msg;

extern void printf(void *);
extern s32 InitSoundSep(s32, s32, s32, s32);

s16 SsSepOpen(s32 data, s16 pitch, s16 count, s16 startCh) {
    s32 mask;
    s16 ch;
    s16 i;
    s32 v0;
    s32 v1;
    s16 *pch;

    pch = &startCh;

    mask = _snd_openflag;
    if (mask == -1) {
        printf(&libsnd_sep_open_limit_msg);
        return -1;
    }
    ch = *pch;
    v0 = 0;
    v1 = 0;
    do {
        if (!(mask & (1 << v0))) {
            ch = (s16)v0;
            v1 = 1;
        }
        v0++;
    } while (!v1);
    _snd_openflag = mask | (1 << ch);
    i = 0;
    if (count > 0) {
        do {
            data += InitSoundSep(ch, i, pitch, data);
            i++;
        } while (i < count);
    }
    return ch;
}
