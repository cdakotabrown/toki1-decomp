/* @CompileEnv: IDO52_FORM_SPLIT=const:0x30000:92 IDO52_UGEN_TEMP_REG=18:t4,19:t5,20:t6 */
/* tt_scroll_speed_step - TT overlay, Address: 0x8013D150, Size: 400 bytes. */
/* PostProc; hook-dependent via @CompileEnv: */
#include "types.h"

extern u8 *tt_step_wk;
extern s32 *tt_scroll_wk;
extern u8 *tt_stage_wk;
extern u8 *tt_player_wk;

void tt_scroll_speed_step(void)
{
    u8 *res;
    s32 *spd;
    u8 *ctl;
    u8 *st;
    s32 v;
    s32 val;
    s32 cur;
    s32 d;
    s32 a;
    u32 x;

    res = tt_step_wk;
    spd = tt_scroll_wk;
    ctl = tt_stage_wk;
    st = tt_player_wk;
    if (res[28] == 16) {
        *(u16 *)(res + 50) += 1;
        x = *(u16 *)(res + 52);
        if (x < 257) {
            *(u16 *)(res + 52) = 256;
        } else {
            *(u16 *)(res + 52) = x - 32;
        }
    } else if (res[28] == 17) {
        *(u16 *)(res + 50) += 1;
        x = *(u16 *)(res + 52);
        if (x >= 4096) {
            *(u16 *)(res + 52) = 4096;
            res[28] = 2;
        } else {
            *(u16 *)(res + 52) = x + 64;
        }
    }
    v = 0;
    if ((u32)*(u16 *)(ctl + 2) >= 1025) {
        spd[1] = 0;
    } else {
        val = *(s16 *)(st + 22);
        if (val < 80) {
            spd[1] = (s32)(u8 *)0x30000;
        } else {
            spd[1] = 0x28000 - (((val - 80) / 32) << 15);
        }
        v = spd[1];
        if (v < 0x8000) {
            spd[1] = 0x8000;
            v = 0x8000;
        }
        if (v > 0x30000) {
            v = 0x30000;
            spd[1] = v;
        }
    }
    cur = spd[0];
    if (v < cur) {
        spd[0] = cur - 1024;
    } else {
        spd[0] = cur + 512;
    }
    d = spd[0] - spd[1];
    a = (d < 0) ? -d : d;
    if (a < 4096) {
        spd[0] = spd[1];
    }
}
