/* Function: Snd_crescendo (Snd_crescendo) */
/* Address: 0x80091BA0 */
/* Size: 1480 bytes (370 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @CompileFlags: -cckr -Wc,-pic1 */
/* @CompileEnv: IDO52_NO_CSE_OPCODES=mpy IDO52_AS1_XBB_SKIP_CAND=all IDO52_AS1_DIV_MACRO=aspsx IDO52_AS1_NO_PASS1_PROP=23,252,253 IDO52_FORCE_WEB_COLOR=6:t0 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: remap_stack_slots --map 20:36 */
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

void Snd_crescendo(ch, idx)
s16 ch;
s16 idx;
{
    u16 l[1];
    u16 r[1];
    Voice *v;
    Voice **slot[1];
    u32 t;

    slot[0] = &_ss_score[ch]; v = &(*slot[0])[idx];
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
                if (l[0] + 1 < 128 && r[0] + 1 < 128) {
                    SpuVmSetSeqVol(ch, l[0] + 1, r[0] + 1, 0);
                } else {
                    SpuVmSetSeqVol(ch, 127, 127, 0);
                    (*slot[0])[idx].flags &= ~0x10;
                }
            } else {
                SpuVmSetSeqVol(ch, 127, 127, 0);
                (*slot[0])[idx].flags &= ~0x10;
            }
            t = v->tick;
        } else if (v->dir < 0) {
            v->cnt++;
            if (v->cnt <= 0) {
                SpuVmGetSeqVol(ch, l, r);
                if (l[0] - 1 >= 0 && r[0] - 1 >= 0) {
                    SpuVmSetSeqVol(ch, l[0] - 1, r[0] - 1, 0);
                } else {
                    SpuVmSetSeqVol(ch, 0, 0, 0);
                    (*slot[0])[idx].flags &= ~0x10;
                }
            } else {
                SpuVmSetSeqVol(ch, 0, 0, 0);
                (*slot[0])[idx].flags &= ~0x10;
            }
            t = v->tick;
        }
        if (t == 0 || v->cnt == 0) {
            (*slot[0])[idx].flags &= ~0x10;
        }
    } else {
        if (v->dir > 0) {
            v->cnt += v->step;
            SpuVmGetSeqVol(ch, l, r);
            if (v->cnt >= 0) {
                if (l[0] - v->step < 128 && r[0] - v->step < 128) {
                    SpuVmSetSeqVol(ch, l[0] - v->step, r[0] - v->step, 0);
                } else {
                    SpuVmSetSeqVol(ch, 127, 127, 0);
                    (*slot[0])[idx].flags &= ~0x10;
                }
            } else {
                SpuVmSetSeqVol(ch, 127, 127, 0);
                (*slot[0])[idx].flags &= ~0x10;
            }
            t = v->tick;
        } else if (v->dir < 0) {
            v->cnt -= v->step;
            SpuVmGetSeqVol(ch, l, r);
            if (v->cnt <= 0) {
                if (l[0] >= -v->step && r[0] >= -v->step) {
                    SpuVmSetSeqVol(ch, l[0] + v->step, r[0] + v->step, 0);
                } else {
                    SpuVmSetSeqVol(ch, 0, 0, 0);
                    (*slot[0])[idx].flags &= ~0x10;
                }
            } else {
                SpuVmSetSeqVol(ch, 0, 0, 0);
                (*slot[0])[idx].flags &= ~0x10;
            }
            t = v->tick;
        }
        if (t == 0 || v->cnt == 0) {
            (*slot[0])[idx].flags &= ~0x10;
        }
    }
tail:
    SpuVmGetSeqVol(ch, &v->pan_l, &v->pan_r);
}
