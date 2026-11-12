/* @SymbolStart: 0x8013BB94 */
/* @SymbolSize: 496 */
/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=all IDO52_FORCE_WEB_SPILL=59 */
#include "types.h"

void device_cmd_send();
void bg_layer_scroll();
void camera_clamp();
void taiiku_cw97_steer_set();

extern volatile u16 taiiku_tama_gclk;
#define STEP taiiku_tama_gclk
extern s32 taiiku_tama_vel;
extern s16 taiiku_tama_sx;
extern s16 taiiku_tama_sy;
s16 taiiku_tama_gdst[2] = {0};
extern u16 taiiku_tama_gcue;
extern u16 taiiku_tama_gfrm_tick[];
extern s16 cw97_x;

void taiiku_tama_girl5_event_step()
{
    s16 p[2];
    s32 d;

    if (STEP == 1) {
        d = (taiiku_tama_sx >> 3) - cw97_x;
        if ((d > 32 && taiiku_tama_vel > 0) || (d < -32 && taiiku_tama_vel < 0)) {
            device_cmd_send(2050);
            taiiku_tama_gcue = 2;
        }
    }
    if (STEP >= 136) {
        STEP = 0;
    }
    bg_layer_scroll(0x12000);
    camera_clamp();
    if ((STEP & 0x1F) == 0) {
        taiiku_cw97_steer_set(0xC000);
    }
    if ((STEP & 0xF) == 0) {
        p[0] = 0;
        p[1] = (taiiku_tama_sx >> 3);
        p[1] += (taiiku_tama_sx >> 3) * 79 / 48;
        taiiku_tama_gdst[1] = (taiiku_tama_sy >> 3) + 16;
        taiiku_tama_gdst[0] = p[1];
    }
    if (taiiku_tama_gcue == 2 && STEP >= 121) {
        taiiku_tama_gcue = 0;
    }
    if (taiiku_tama_gfrm_tick[1] != 0 && taiiku_tama_gfrm_tick[1] % 90 == 0) {
        if (taiiku_tama_gcue == 0) {
            taiiku_tama_gcue = 1;
            device_cmd_send(2048);
            STEP += 1;
        } else if (taiiku_tama_gcue == 1) {
            taiiku_tama_gcue = 0;
            device_cmd_send(2049);
            STEP += 1;
        }
    }
}
