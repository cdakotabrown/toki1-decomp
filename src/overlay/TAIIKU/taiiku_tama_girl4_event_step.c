/* @CompileFlags: -cckr -Wc,-pic0 */
/* @CompileEnv: IDO52_COPYPROP_VAR=-12 IDO52_AS1_TIE_TAKE=7 IDO52_FORCE_WEB_COLOR=0:v1 */
/* @SymbolStart: 0x8013B9D4 */
/* @SymbolSize: 448 */
/* Address: 0x8013B9DC (window starts at the absorbed cross-prologue head */
/* Size: 448 bytes (112 insns) including the head */
#include "types.h"

extern volatile u16 taiiku_tama_gclk;
extern u16 taiiku_tama_gcue;
s16 taiiku_tama_gdst[2] = { 0, 0 };
extern s16 taiiku_tama_sx;
extern s16 taiiku_tama_sy;
extern s32 taiiku_tama_vel;
extern s16 cal_cw_x;
extern s16 cal_cw_y;
extern s16 cw97_x;

extern void device_cmd_send();
extern void taiiku_cw97_steer_set();
extern void bg_layer_scroll();
extern void camera_clamp();

void taiiku_tama_girl4_event_step()
{
    union { int w; s16 h[2]; } u;
    unsigned step;
    s16 *p;
    int lo;
    int hi;

    step = taiiku_tama_gclk;
    if (step >= 560) {
        taiiku_tama_gcue = 0;
        device_cmd_send(2050);
        step = 0;
    }
    taiiku_tama_gclk = step;
    bg_layer_scroll(0x15000);
    camera_clamp();
    step = taiiku_tama_gclk;
    if ((step & 4) == 0) {
        taiiku_cw97_steer_set(0xF000);
        step = taiiku_tama_gclk;
    }
    if ((step & 0xC) == 0) {
        p = u.h;
        if (step >= 551 && step < 600) {
            lo = cw97_x;
            hi = cal_cw_x;
            if (lo + 30 < hi) {
                taiiku_tama_gdst[0] = hi - 30;
            } else if (hi < lo - 30) {
                taiiku_tama_gdst[0] = hi + 30;
            }
            taiiku_tama_gdst[1] = cal_cw_y;
        } else {
            p[0] = 0;
            p[1] = taiiku_tama_sx >> 3;
            u.w += taiiku_tama_vel * 80;
            taiiku_tama_gdst[0] = p[1];
            taiiku_tama_gdst[1] = (taiiku_tama_sy >> 3) + 16;
        }
    }
    taiiku_tama_gclk = step;
    if (step == 0) {
        return;
    }
    taiiku_tama_gclk = step;
    if (step % 35 != 0) {
        return;
    }
    if (taiiku_tama_gcue == 0) {
        taiiku_tama_gcue = 1;
        device_cmd_send(2048);
    } else {
        taiiku_tama_gcue = 0;
        device_cmd_send(2049);
    }
    step = taiiku_tama_gclk;
    taiiku_tama_gclk = step + 1;
}
