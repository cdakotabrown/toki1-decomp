/* @SymbolStart: 0x8013BF34 */
/* @SymbolSize: 524 */
/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=46:v1 IDO52_UGEN_NO_TARGET=5 */
#include "types.h"

typedef struct {
    u16 h0;
    u16 h2;
    s16 h4;
    s16 h6;
    s32 w8;
    s32 wC;
    s16 h10;
    u16 h12;
} TaiState;

TaiState taiiku_tama_gfrm_tick = {0};
extern s32 taiiku_tama_vel;
extern s16 taiiku_tama_sx;
extern s16 taiiku_tama_sy;
extern s16 cw97_frame;
extern s32 taiiku_course_no;

void device_cmd_send();
void taiiku_cw97_steer_set();
void bg_layer_scroll();
void camera_clamp();

void taiiku_tama_girl7_event_step()
{
    s32 x;
    s32 y;

    if (taiiku_tama_gfrm_tick.h2 == 300) {
        taiiku_tama_gfrm_tick.h10 = 1;
        if (taiiku_tama_gfrm_tick.h12 == 0) {
            taiiku_tama_gfrm_tick.h12 = 1;
            device_cmd_send(2048);
        } else {
            taiiku_tama_gfrm_tick.h12 = 0;
            device_cmd_send(2049);
        }
        taiiku_tama_gfrm_tick.w8 = 0;
        taiiku_tama_gfrm_tick.wC = 0;
        taiiku_tama_gfrm_tick.h2++;
        cw97_frame = 124;
        camera_clamp();
    } else if (taiiku_tama_gfrm_tick.h2 == 303) {
        cw97_frame = 125;
        camera_clamp();
    } else if (taiiku_tama_gfrm_tick.h2 == 305) {
        cw97_frame = 126;
        camera_clamp();
    } else if (taiiku_tama_gfrm_tick.h2 < 421 && taiiku_tama_gfrm_tick.h2 > 300) {
        camera_clamp();
    } else if (taiiku_tama_gfrm_tick.h2 >= 421) {
        camera_clamp();
        taiiku_tama_gfrm_tick.h10 = 0;
        taiiku_tama_gfrm_tick.h2 = 0;
    } else {
        bg_layer_scroll(taiiku_tama_gfrm_tick.h2 * 0 + 0x12000);
        camera_clamp();
        if (!(taiiku_tama_gfrm_tick.h2 & 8)) {
            taiiku_cw97_steer_set(0xE000);
        }
        if (!(taiiku_tama_gfrm_tick.h2 & 0xF)) {
            x = taiiku_tama_sx >> 3;
            y = x * 79 / 48;
            y = (s16)(x + y);
            if (taiiku_course_no % 3 != 0 && (taiiku_tama_sx >= 0 || taiiku_tama_vel > 0)) {
                y = (s16)(x + 32);
            }
            taiiku_tama_gfrm_tick.h4 = y;
            taiiku_tama_gfrm_tick.h6 = (taiiku_tama_sy >> 3) + 16;
        }
    }
}
