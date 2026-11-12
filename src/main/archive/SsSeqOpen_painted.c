/* Function: func_8008FBD8 */
/* Address: 0x8008FBD8 */
/* Size: 156 bytes (39 instructions) */
/* Region: IDO (Library, >= 0x80086000) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: custom_script custom_func_8008FBD8.py */
#include "types.h"

extern s32 _snd_openflag;
extern char libsnd_seq_open_limit_msg;

void printf();
void InitSoundSeq();

s32 func_8008FBD8(s32 arg0, s16 arg1, s32 arg2, s32 mask) {
    s16 a0_local;
    s32 i;
    s32 found;

    arg2 = arg0;
    if (mask == -1) {
        printf(&libsnd_seq_open_limit_msg);
        return -1;
    }

    a0_local = 0;
    i = 0;
    found = 0;
    do {
        if (!(mask & (1 << i))) {
            a0_local = (s16)i;
            found = 1;
        }
        i++;
    } while (found == 0);

    mask |= (1 << a0_local);
    _snd_openflag = mask;
    InitSoundSeq();
    return a0_local;
}
