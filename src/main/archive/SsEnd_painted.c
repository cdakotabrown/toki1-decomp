/* Function: SsEnd */
/* Address: 0x8008BE0C */
/* Size: 176 bytes (44 instructions) */
/* Region: Library (0x80086000+) */
/* Likely Symbol: set_tarao_rect (from o.bin) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: custom_script custom_func_8008BE0C.py */
#include "types.h"

extern s32 _snd_seq_no_tick;
extern s32 _snd_1per2;
extern s32 _snd_use_vsync_cb;
extern s32 _snd_use_interrupt_id;
extern s32 _snd_vsync_cb;

extern void EnterCriticalSection(void);
extern void VSyncCallback(s32);
extern void InterruptCallback(s32, s32);
extern void ExitCriticalSection(void);

void SsEnd(void) {
    s32 mode;

    if (_snd_seq_no_tick != 0) {
        return;
    }
    _snd_1per2 = 0;
    EnterCriticalSection();

    if (_snd_use_vsync_cb != 0) {
        VSyncCallback(0);
        _snd_use_vsync_cb = 0;
    } else {
        mode = _snd_use_interrupt_id;
        if (mode != -1) {
            if (mode == 0) {
                InterruptCallback(0, _snd_vsync_cb);
            } else {
                _snd_use_interrupt_id = mode;
                InterruptCallback(mode, 0);
            }
            _snd_use_interrupt_id = -1;
        }
    }
    ExitCriticalSection();
}
