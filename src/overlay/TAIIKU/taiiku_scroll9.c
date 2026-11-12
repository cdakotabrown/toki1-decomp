/* Address: 0x8013C32C */
/* Size: 600 bytes (150 insns) */
/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=0:a1,64:a1 */
#include "types.h"

extern void bg_layer_scroll(int arg0);
extern void camera_clamp(void);
extern void taiiku_cw97_steer_set(int arg0);
extern void device_cmd_send(int arg0);
extern s32 abs(int arg0);

extern u16 taiiku_tama_gclk;
extern volatile u16 taiiku_tama_gclk_v;
extern s32 taiiku_tama_vel;
extern s16 taiiku_tama_sx;
extern s16 taiiku_tama_sy;
s16 taiiku_tama_gdst[2] = {0, 0};
extern u16 taiiku_tama_gcue;
extern s16 cal_cw_x;
extern s16 cal_cw_y;
extern s16 cw97_x;
extern s16 cw97_y;
extern u8 class_w[];

void taiiku_scroll9(void) {
    s32 w;
    u32 a1;

    bg_layer_scroll(0x11800);
    camera_clamp();
    a1 = taiiku_tama_gclk_v;
    if (!(a1 & 8)) {
        taiiku_cw97_steer_set(0x8000);
        a1 = taiiku_tama_gclk_v;
    }
    if (!(a1 & 0xF)) {
        ((s16 *)&w)[0] = 0;
        ((s16 *)&w)[1] = taiiku_tama_sx >> 3;
        w += taiiku_tama_vel * 80;
        taiiku_tama_gdst[0] = ((s16 *)&w)[1];
        taiiku_tama_gdst[1] = (taiiku_tama_sy >> 3) + 16;
    }
    if (cal_cw_x < cw97_x) {
        if (abs(cal_cw_y - cw97_y) < 12 &&
            cw97_x - 32 < cal_cw_x) {
            *(volatile s32 *)(class_w + 6632) += 0x11800;
            goto clamp;
        }
    }
    if (*(volatile s16 *)(class_w + 6634) < cal_cw_x) {
        if (abs(cal_cw_y - cw97_y) < 12 &&
            cal_cw_x < *(volatile s16 *)(class_w + 6634) + 32) {
            *(volatile s32 *)(class_w + 6632) += 0xFFFEE800;
        }
    }
clamp:
    if (taiiku_tama_gclk >= 129U) {
        taiiku_tama_gclk = 32;
    }
    if (taiiku_tama_gclk != 0 && !(taiiku_tama_gclk & 0x1F) && taiiku_tama_gclk >= 33U) {
        a1 = taiiku_tama_gcue;
        if (a1 == 0) {
            taiiku_tama_gcue = 1;
            taiiku_tama_gclk++;
            device_cmd_send(2048);
        } else if (a1 == 1) {
            taiiku_tama_gcue = 0;
            taiiku_tama_gclk++;
            device_cmd_send(2049);
        } else {
            taiiku_tama_gcue = 0;
        }
    }
}
