/* Address: 0x8013C140 */
/* Size: 492 bytes (123 insns) */
/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=all IDO52_FORCE_WEB_COLOR=0:v1,70:v0 */
/* @PostProc: patch_insn_reg_field 0x1e0:rt:t9:v1 0x1e8:rt:t9:v1 */
#include "types.h"

extern void bg_layer_scroll(int arg0);
extern void camera_clamp(void);
extern void taiiku_cw97_steer_set(int arg0);
extern void device_cmd_send(int arg0);

extern volatile u16 taiiku_tama_gclk;
extern s32 taiiku_tama_vel;
extern s16 taiiku_tama_sx;
extern s16 taiiku_tama_sy;
extern s32 taiiku_tama_fall;
s16 taiiku_tama_gdst[2] = {0, 0};
extern u16 taiiku_tama_gcue;
extern s16 cw97_x;

void taiiku_scroll8(void) {
    s32 w;
    u32 v1;

    bg_layer_scroll(0x15000);
    camera_clamp();
    v1 = taiiku_tama_gclk;
    if (!(v1 & 4)) {
        taiiku_cw97_steer_set(0xF000);
        v1 = taiiku_tama_gclk;
    }
    if (!(v1 & 0xC)) {
        ((s16 *)&w)[0] = 0;
        ((s16 *)&w)[1] = taiiku_tama_sx >> 3;
        w += taiiku_tama_vel * 80;
        taiiku_tama_gdst[0] = ((s16 *)&w)[1];
        taiiku_tama_gdst[1] = (taiiku_tama_sy >> 3) + 16;
    }
    if (taiiku_tama_gcue != 0 && v1 >= 65) {
        taiiku_tama_gcue = 0;
    }
    if (taiiku_tama_fall == 0 && !(v1 & 0x1F) && taiiku_tama_gcue == 0) {
        if (taiiku_tama_vel > 0) {
            if (cw97_x < (taiiku_tama_sx >> 3) - 24) {
                taiiku_tama_gcue = 1;
                taiiku_tama_gclk = 0;
                device_cmd_send(2050);
            } else {
                device_cmd_send(2049);
            }
            v1 = taiiku_tama_gclk;
            v1 = v1 + 1;
        } else {
            if (taiiku_tama_vel < 0) {
                if ((taiiku_tama_sx >> 3) + 24 < cw97_x) {
                    taiiku_tama_gcue = 1;
                    taiiku_tama_gclk = 0;
                    device_cmd_send(2050);
                    v1 = taiiku_tama_gclk;
                    v1 = v1 + 1;
                    goto trunc;
                }
                device_cmd_send(2048);
                v1 = taiiku_tama_gclk;
            }
            v1 = v1 + 1;
        }
trunc:
        v1 = v1 & 0xFFFF;
    }
    taiiku_tama_gclk = v1;
    if (v1 >= 129) {
        taiiku_tama_gclk = 64;
    }
}
