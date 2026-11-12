/* Function: _SsStart */
/* Address: 0x8008BB24 */
/* Size: 680 bytes (170 instructions) */
/* Region: Library (0x80086000+) */
/* @CompileFlags: -Wc,-pic0 -Wo,-loopunroll,0 */
/* @CompileEnv: IDO52_AS1_DIV_MACRO=aspsx IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_COPYPROP_SITE=lod:4:deny IDO52_AS1_XBB_SKIP_CAND=all IDO52_AS1_NO_FALLTHRU_FILL=all IDO52_AS1_KEEP_JUMP_NEXT=all IDO52_KEEP_JUMP_NEXT=lab:14 IDO52_AS1_NO_PASS1_PROP=11 IDO52_AS1_FORCE_BOPT=2 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: remap_stack_slots --map '28:36,26:32,20:28' */
/* @PostProc: convert_or_to_addu */
#include "types.h"

extern s32 SsFCALL;
extern s32 _snd_seq_no_tick;
extern s32 _snd_use_vsync_cb;
extern s32 _snd_use_interrupt_id;
extern s32 _snd_1per2;
extern s32 _snd_vsync_cb;
extern void SsSeqCalledTbyT();

void VSyncCallback(u8 *);
void EnterCriticalSection(void);
void ResetRCnt(u32, u16);
void SetRCnt(u32, u16, s32);
void _wait(void);
void StartRCnt(u32);
s32 InterruptCallback(u32, u8 *);
void ExitCriticalSection(void);
void _SsTrapIntrVSync(void);
void _SsSeqCalledTbyT_1per2(void);

void _SsStart(s32 arg0) {
    s32 state;
    s32 sel;
    u32 cmd;
    u16 freq;
    s32 i;

    for (i = 0; i != 1000; i += 4) {
    }

    state = SsFCALL;

    sel = state;
    switch (sel) {
    case 5:
        if (arg0 == 0) {
            _snd_use_vsync_cb++;
        } else {
            cmd = 0xF2000003;
            freq = 1;
            _snd_use_interrupt_id = 0;
        }
        break;
    case 3:
        cmd = 0xF2000002;
        freq = 0x89D0;
        _snd_use_interrupt_id = 6;
        break;
    case 2:
        cmd = 0xF2000002;
        freq = 17640;
        _snd_use_interrupt_id = 6;
        break;
    default:
    case 1:
    case 4:
        if (_snd_seq_no_tick != 0) {
            return;
        }
        cmd = 0xF2000002;
        _snd_use_interrupt_id = 6;
        if (state < 70) {
            freq = 0x204CC0 / state;
            _snd_1per2++;
        } else {
            freq = 0x409980 / state;
        }
        break;
    case 0:
        return;
    }

    if (_snd_use_vsync_cb != 0) {
        VSyncCallback((u8 *)SsSeqCalledTbyT);
        return;
    }

    EnterCriticalSection();
    ResetRCnt(cmd, freq);
    SetRCnt(cmd, freq, 4096);
    _wait();
    _wait();
    StartRCnt(cmd);

    if (_snd_use_interrupt_id == 0) {
        _snd_vsync_cb = InterruptCallback(0, 0);
        InterruptCallback(_snd_use_interrupt_id, (u8 *)_SsTrapIntrVSync);
    } else if (_snd_1per2 == 0) {
        InterruptCallback(_snd_use_interrupt_id, (u8 *)SsSeqCalledTbyT);
    } else {
        InterruptCallback(_snd_use_interrupt_id, (u8 *)_SsSeqCalledTbyT_1per2);
    }
    ExitCriticalSection();
}
