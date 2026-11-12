/* Function: _SsSeqCalledTbyT_1per2 */
/* Address: 0x8008B344 */
/* Size: 68 bytes (17 instructions) */
/* Region: IDO (Library, >= 0x80086000) */
/* @CompileEnv: IDO52_AS1_NO_COPY_PROP=all IDO52_AS1_NO_PASS1_PROP=all */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
#include "types.h"

extern s32 libsnd_seq_tick_toggle;
extern void SsSeqCalledTbyT(void);

void _SsSeqCalledTbyT_1per2(void) {
    if (libsnd_seq_tick_toggle == 0) {
        libsnd_seq_tick_toggle = libsnd_seq_tick_toggle + 1;
        return;
    }
    libsnd_seq_tick_toggle = 0;
    SsSeqCalledTbyT();
}
