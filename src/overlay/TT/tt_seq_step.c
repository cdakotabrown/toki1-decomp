/* tt_seq_step - TT overlay, Address: 0x80142AE0, Size: 384 bytes. */
#include "types.h"

extern u8 *tt_boss_wk;

void tt_seq_step(s16 *seq, s32 idx)
{
    u8 *g;
    u8 *e;
    s16 *vp;
    s32 cnt;
    s32 val;

    g = tt_boss_wk;
    e = g + 56 + idx * 20;
    if (g[0] == 0) {
        return;
    }
    if ((tt_boss_wk + 784)[idx] == 0) {
        return;
    }
    if (e[8] & 0x80) {
        e[8] = e[8] & 0xFF7F;
        e[6] = seq[0];
        e[9] = seq[1];
        g[808 + idx] = 0;
        return;
    }
    if (e[8] & 0x20) {
        e[8] = e[8] & 0xFFDF;
        g[808 + idx] = 0;
        return;
    }
    if (e[9] == 0) {
        cnt = tt_boss_wk[808 + idx] + 2;
        vp = &((0, seq))[cnt];
        val = vp[0];
        if (val == -1) {
            e[8] = e[8] | 1;
            e[8] = e[8] | 0x40;
            e[6] = vp[-2];
        } else if (val == -2) {
            e[6] = seq[0];
            e[9] = seq[1];
            cnt = 0;
        } else if (val == -3) {
            g[784 + idx] = 0;
            e[8] = e[8] | 1;
        } else {
            e[6] = val;
            e[9] = vp[1];
        }
        g[808 + idx] = cnt;
    } else {
        e[9] = e[9] - 1;
    }
}
