/* @SymbolStart: 0x80090DA0 */
/* @SymbolSize: 732 */
/* Function: SsSeqCalledTbyT */
/* Address: 0x80090DA8 (true entry 0x80090DA0, see the window above) */
/* Size: 732 bytes (183 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 (t-register temps, `b`, no `j`) */
/* @CompileFlags: -cckr -Wc,-pic0 -Wo,-do_opt_saved_regs */
/* @CompileEnv: IDO52_NO_FINDINDUCT_STRIDE=1 IDO52_SPLIT_BLOCKS=26#1:32+33+34+35+5+6+7+8 IDO52_FORCE_WEB_NOSPILL=26,26.0:50 IDO52_CONST_NOINREG_VALUES=168 IDO52_NO_CSE_SITE=mpy:1-8 IDO52_AS1_NO_FALLTHRU_FILL=all IDO52_AS1_NO_LA_PULL=all */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: remap_stack_slots --map 32:36 */
#include "types.h"

typedef struct {
    u8 pad0[144];
    s32 flags;
    u8 pad148[20];
} Voice;

extern Voice *_ss_score[];
extern s32 _snd_ev_flag;
extern s32 _snd_openflag;
extern s16 _snd_seq_s_max;
extern s16 _snd_seq_t_max;

extern void SpuVmFlush();
extern void Snd_play();
extern void Snd_crescendo();
extern void Snd_decrescendo();
extern void Snd_tempo();
extern void Snd_pause();
extern void Snd_replay();
extern void Snd_stop();

void SsSeqCalledTbyT()
{
    int i;
    int j;
    Voice **slot;
    int flags;

    if (_snd_ev_flag == 1) {
        return;
    }
    _snd_ev_flag = 1;
    SpuVmFlush();
    for (i = 0; i < _snd_seq_s_max; i++) {
        if ((_snd_openflag & (1U << i)) == 0) {
            continue;
        }
        slot = &_ss_score[i];
        for (j = 0; j < _snd_seq_t_max; j++) {
            flags = (*slot)[j].flags;
            if (flags & 1) {
                Snd_play(i, j);
                flags = (*slot)[j].flags;
                if (flags & 0x10) {
                    Snd_crescendo(i, j);
                    flags = (*slot)[j].flags;
                }
                if (flags & 0x20) {
                    Snd_decrescendo(i, j);
                    flags = (*slot)[j].flags;
                }
                if (flags & 0x40) {
                    Snd_tempo(i, j);
                    flags = (*slot)[j].flags;
                }
                if (flags & 0x80) {
                    Snd_tempo(i, j);
                    flags = (*slot)[j].flags;
                }
            }
            if (flags & 2) {
                Snd_pause(i, j);
                flags = (*slot)[j].flags;
            }
            if (flags & 8) {
                Snd_replay(i, j);
                flags = (*slot)[j].flags;
            }
            if (flags & 4) {
                Snd_stop(i, j);
                (*slot)[j].flags = 0;
            }
        }
    }
    _snd_ev_flag = 0;
}
