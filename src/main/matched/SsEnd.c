/* Address: 0x8008BE0C  (true start; 0x8008BE14 is the PROLOGUE) */
/* Size: 176 bytes (44 instructions) */
/* Region: Library (0x80086000+) */
/* Compiler: IDO 5.2 */
/* @SymbolStart: 0x8008BE0C */
/* @SymbolSize: 176 */
/* @CompileEnv: IDO52_AS1_NO_LA_PULL=all IDO52_AS1_NO_FALLTHRU_FILL=all IDO52_UGEN_TEMP_REG=3:a0 IDO52_UGEN_TEMP_REG_INPLACE=3 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
#include "types.h"

extern s32 _snd_seq_no_tick;
extern s32 _snd_1per2;
extern s32 _snd_use_vsync_cb;
extern volatile s32 _snd_use_interrupt_id;
extern s32 _snd_vsync_cb;
extern void EnterCriticalSection(void);
extern void VSyncCallback(s32);
extern void InterruptCallback(s32, s32);
extern void ExitCriticalSection(void);

void SsEnd(void) {
    s32 channel;

    if (_snd_seq_no_tick != 0) {
        return;
    }

    _snd_1per2 = 0;
    EnterCriticalSection();

    if (_snd_use_vsync_cb != 0) {
        VSyncCallback(0);
        _snd_use_vsync_cb = 0;
        goto do_cleanup;
    }

    channel = _snd_use_interrupt_id;
    if (channel != -1) {
        if (channel == 0) {
            InterruptCallback(0, _snd_vsync_cb);
        } else {
            _snd_use_interrupt_id = channel;
            InterruptCallback(channel, 0);
        }
        _snd_use_interrupt_id = -1;
    }
do_cleanup:
    ExitCriticalSection();
}
