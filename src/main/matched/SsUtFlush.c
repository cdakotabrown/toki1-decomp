/* Function: SsUtFlush */
/* Address: 0x80090C00 */
/* Size: 64 bytes */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
#include "types.h"

extern s32 _snd_ev_flag;
extern void SpuVmFlush(void);

void SsUtFlush(void) {
    if (_snd_ev_flag != 1) {
        _snd_ev_flag = 1;
        SpuVmFlush();
        _snd_ev_flag = 0;
    }
}
