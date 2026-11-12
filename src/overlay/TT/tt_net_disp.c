/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_UGEN_NO_TARGET=25,26 IDO52_UGEN_TEMP_REG=22:t7,51:t9,52:t7,57:t7,58:t8 IDO52_FORCE_WEB_COLOR=84:t0,88:a1,162:a1,113:a3,155:a0,176:a0,115:a0,96:ra,118:t5,122:s0,103:s1,224:fp */
/* @PostProc: swap_instructions --offset1 0x328 --offset2 0x32c */
/* @PostProc: swap_instructions --offset1 0x3b0 --offset2 0x3b4 */
/* @PostProc: remap_stack_slots --from 88 --to 84 */
#include "types.h"

typedef struct { u32 addr : 24; u32 len : 8; } P_TAG; typedef struct { u8 pad0[24]; u8 **tab; u8 pad1[22]; u16 phase; s16 sway; } NET;

extern NET *tt_step_wk;
extern u8 *tt_disp_wk;
extern u8 *tt_stage_wk;
extern u8 *tt_db_buf;

void tt_net_disp()
{
    u8 *p;
    s32 ctr;
    u8 *w;
    NET *g;
    u8 *base;
    s32 y0, y1, y2;
    s32 b;
    s32 y3, x0, v, x3;
    s16 s, x1, x2;
    s16 ax[17][17];
    s16 ay[17][17];

    w = tt_stage_wk;
    g = tt_step_wk;
    base = tt_db_buf + *(s32 *)(tt_disp_wk + 8) * 32952;
    for (b = 0; b < 17; b++) {
        for (ctr = 0; ctr < 17; ctr++) {
            ax[ctr][b] = (ctr - 8) * (34816 - g->sway) / 1920;
            ay[ctr][b] = (b - 8) * (34816 - g->sway) / 1920;
            ax[ctr][b] += rcos(g->phase * 32 + b * 512) / g->sway;
            ay[ctr][b] += (s16)(rsin(g->phase * 32 + ctr * 512) / g->sway);
        }
    }
    s = 768 - *(u16 *)(w + 2);
    for (b = 0; b < 256; b += 16) {
        for (ctr = 0; ctr < 16; ctr++) {
            p = *(u8 **)(base + 4);
            *(u8 **)(base + 4) = p + 40;
            p[7] = 45;
            p[3] = 9;
            y0 = ay[ctr][b / 16] - (s16)(s & 15);
            y0 = y0 + 128;
            y1 = ay[ctr + 1][b / 16] - (s16)(s & 15);
            y1 = y1 + 128;
            y2 = ay[ctr][b / 16 + 1] - (s16)(s & 15);
            y3 = ay[ctr + 1][b / 16 + 1] - (s16)(s & 15);
            x0 = ax[ctr][b / 16];
            x1 = ax[ctr + 1][b / 16];
            x2 = ax[ctr][b / 16 + 1];
            x3 = ax[ctr + 1][b / 16 + 1];
            *(s16 *)(p + 34) = y3 + 128;
            *(s16 *)(p + 26) = y2 + 128;
            *(s16 *)(p + 18) = y1;
            *(s16 *)(p + 10) = y0;
            *(s16 *)(p + 8) = x0;
            *(s16 *)(p + 16) = x1;
            *(s16 *)(p + 24) = x2;
            *(s16 *)(p + 32) = x3;
            x3 = (s32)g->tab;
            x0 = (s32)(*(u8 **)((u8 *)x3 + 4) +
                *(u8 *)(*(u8 **)x3 + ((b + ctr + (s & -16)) & 1023)) * 3);
            y0 = ((u8 *)x0)[1] & 7;
            y0 = (y0 << 4) & 255;
            v = ((u8 *)x0)[1] >> 3;
            v = (v << 4) & 255;
            if (((u8 *)x0)[0] == 1) {
                v = (v + 128) & 255;
            }
            p[29] = v + 15;
            p[37] = v + 15;
            p[20] = y0 + 15;
            p[36] = y0 + 15;
            p[12] = y0;
            p[13] = v;
            p[21] = v;
            p[28] = y0;
            *(s16 *)(p + 22) = 14;
            *(s16 *)(p + 14) = (((((u8 *)x0)[2] >> 4) + 240) << 6) + (((u8 *)x0)[2] & 15);
            ((P_TAG *)p)->addr = ((P_TAG *)(base + 0x8000 + 60))->addr;
            ((P_TAG *)(base + 0x8000 + 60))->addr = (u32)p;
        }
    }
}
