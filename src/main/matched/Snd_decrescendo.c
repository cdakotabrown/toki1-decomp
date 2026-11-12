/* Address: 0x80092170 */
/* Size: 1600 bytes (400 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* load-delay nop.  Measured with it appended to @CompileEnv: 29/400 Body */
/* @CompileFlags: -cckr -Wc,-pic1 */
/* @CompileEnv: IDO52_NO_CSE_OPCODES=mpy IDO52_AS1_XBB_SKIP_CAND=all IDO52_AS1_DIV_MACRO=aspsx IDO52_AS1_NO_PASS1_PROP=23,205,206 IDO52_FORCE_WEB_COLOR=36:v1 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: remap_stack_slots --keep-frame --map 20:36,68:60,72:64,76:68,80:72,84:76 */
/* @PostProc: patch_stack_complete --old 80 --new 72 */
#include "types.h"

typedef struct {
    u8 pad0[60];
    s16 dir;
    s16 cnt;
    s16 step;
    u8 pad66[52];
    u16 pan_l;
    u16 pan_r;
    u8 pad122[22];
    s32 flags;
    u8 pad148[4];
    u32 tick;
    u8 pad156[12];
} Voice;

extern Voice *_ss_score[];

extern void SpuVmSetSeqVol();
extern void SpuVmGetSeqVol();

void Snd_decrescendo(ch, idx)
s16 ch;
s16 idx;
{
    u16 l[1];
    u16 r[1];
    Voice *v;
    Voice **slot;
    u32 t;

    slot = &_ss_score[ch];
    v = &(*slot)[idx];
    v->tick--;
    t = v->tick;
    if (v->step > 0) {
        if (t % v->step != 0) {
            goto tail;
        }
        if (v->dir > 0) {
            v->cnt--;
            if (v->cnt >= 0) {
                SpuVmGetSeqVol(ch, l, r);
                if (l[0] != 0 && r[0] != 0) {
                    SpuVmSetSeqVol(ch, l[0] - 1, r[0] - 1, 0);
                } else {
                    SpuVmSetSeqVol(ch, 0, 0, 0);
                    (*slot)[idx].flags &= ~0x20;
                }
            } else {
                SpuVmSetSeqVol(ch, 0, 0, 0);
                (*slot)[idx].flags &= ~0x20;
            }
            t = v->tick;
        } else if (v->dir < 0) {
            v->cnt++;
            if (v->cnt <= 0) {
                SpuVmGetSeqVol(ch, l, r);
                if (l[0] + 1 < 128 && r[0] + 1 < 128) {
                    SpuVmSetSeqVol(ch, l[0] + 1, r[0] + 1, 0);
                } else {
                    SpuVmSetSeqVol(ch, 127, 127, 0);
                    (*slot)[idx].flags &= ~0x20;
                }
            } else {
                SpuVmSetSeqVol(ch, 127, 127, 0);
                (*slot)[idx].flags &= ~0x20;
            }
            t = v->tick;
        }
        if (t == 0 || v->cnt == 0) {
            (*slot)[idx].flags &= ~0x20;
        }
    } else {
        if (v->dir > 0) {
            v->cnt += v->step;
            if (v->cnt >= 0) {
                SpuVmGetSeqVol(ch, l, r);
                if (l[0] >= -v->step && r[0] >= -v->step) {
                    SpuVmSetSeqVol(ch, l[0] + v->step, r[0] + v->step, 0);
                } else {
                    SpuVmSetSeqVol(ch, 0, 0, 0);
                    (*slot)[idx].flags &= ~0x20;
                }
            } else {
                SpuVmSetSeqVol(ch, 0, 0, 0);
                (*slot)[idx].flags &= ~0x20;
            }
            t = v->tick;
        } else if (v->dir < 0) {
            v->cnt -= v->step;
            if (v->cnt <= 0) {
                SpuVmGetSeqVol(ch, l, r);
                if (l[0] - v->step < 128 && r[0] - v->step < 128) {
                    SpuVmSetSeqVol(ch, l[0] - v->step, r[0] - v->step, 0);
                } else {
                    SpuVmSetSeqVol(ch, 127, 127, 0);
                    (*slot)[idx].flags &= ~0x20;
                }
            } else {
                SpuVmSetSeqVol(ch, 127, 127, 0);
                (*slot)[idx].flags &= ~0x20;
            }
            t = v->tick;
        }
        if (t == 0 || v->cnt == 0) {
            (*slot)[idx].flags &= ~0x20;
        }
    }
tail:
    SpuVmGetSeqVol(ch, &v->pan_l, &v->pan_r);
}
