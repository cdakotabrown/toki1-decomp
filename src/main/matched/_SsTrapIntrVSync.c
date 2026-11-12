/* Function: _SsTrapIntrVSync */
/* Address: 0x8008B310 */
/* Size: 52 bytes (13 instructions) */
/* Region: IDO (0x80086000+) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: swap_registers_liveness v1->v0 */
#include "types.h"

extern void SsSeqCalledTbyT(void);
extern void (*_snd_vsync_cb)(void);

void _SsTrapIntrVSync(void) {
    void (*ptr)(void) = _snd_vsync_cb;
    if (ptr) {
        (*ptr)();
    }
    SsSeqCalledTbyT();
}
