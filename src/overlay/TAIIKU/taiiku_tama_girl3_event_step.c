/* @SymbolStart: 0x8013B850 */
/* @SymbolSize: 388 */
/* @CompileFlags: -cckr */
#include "types.h"

void device_cmd_send();
void bg_layer_scroll();
void camera_clamp();
void taiiku_cw97_steer_set();

extern u16 taiiku_tama_gclk;
extern s32 taiiku_tama_vel;
extern s16 taiiku_tama_sx;
extern s16 taiiku_tama_sy;
s16 taiiku_tama_gdst[2] = {0};
extern u16 taiiku_tama_gcue;
extern s32 taiiku_course_no;

void taiiku_tama_girl3_event_step()
{
    s16 p[2];

    if (taiiku_tama_gclk >= 600) {
        taiiku_tama_gcue = 0;
        device_cmd_send(2050);
        taiiku_tama_gclk = 0;
    }
    bg_layer_scroll(0x10000);
    camera_clamp();
    if ((taiiku_tama_gclk & 8) == 0) {
        taiiku_cw97_steer_set(0x9000);
    }
    if ((taiiku_tama_gclk & 0xF) == 0) {
        p[0] = 0;
        p[1] = (taiiku_tama_sx >> 3);
        p[1] += (taiiku_tama_sx >> 3) * 80 / 48;
        if (taiiku_course_no % 3 != 0) {
            if (taiiku_tama_sx >= 0 || taiiku_tama_vel > 0) {
                p[1] = (taiiku_tama_sx >> 3) + 32;
            }
        }
        taiiku_tama_gdst[0] = p[1];
        taiiku_tama_gdst[1] = (taiiku_tama_sy >> 3) + 16;
    }
    if (taiiku_tama_gclk != 0 && taiiku_tama_gclk % 45 == 0) {
        if (taiiku_tama_gcue == 0) {
            taiiku_tama_gcue = 1;
            device_cmd_send(2048);
        } else {
            taiiku_tama_gcue = 0;
            device_cmd_send(2049);
        }
        taiiku_tama_gclk++;
    }
}
