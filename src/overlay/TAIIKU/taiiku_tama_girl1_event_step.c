/* @SymbolStart: 0x8013B5F8 */
/* @SymbolSize: 528 */
/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=all IDO52_FORCE_WEB_COLOR=41:v1 */
/* @PostProc: patch_insn_reg_field 0x198:rt:v0:t1 0x1a4:rs:v0:t1 0x1a4:rt:v0:t1 0x1ac:rs:v0:t1 0x1b4:rs:v0:t1 0x1b8:rt:t2:t3 0x1bc:rd:t1:t2 0x1c0:rs:t1:t2 0x1c8:rs:t2:t3 0x1c8:rt:t2:t3 0x1d0:rs:t2:t3 0x1d8:rt:t3:t4 0x1e0:rt:t3:t4 0x1fc:rt:v0:t5 0x200:rs:v0:t5 0x200:rt:v0:t5 0x208:rs:v0:t5 0x208:rt:t4:t6 0x20c:rt:t4:t6 */
#include "types.h"

void device_cmd_send();
void bg_layer_scroll();
void camera_clamp();
void taiiku_cw97_steer_set();

extern volatile u16 taiiku_tama_gclk;
extern s32 taiiku_tama_vel;
extern s16 taiiku_tama_sx;
extern s16 taiiku_tama_sy;
s16 taiiku_tama_gdst[2] = {0};
s32 taiiku_tama_gpush[2] = {0};
extern u16 taiiku_tama_ghold;
extern u16 taiiku_tama_gcue;
extern s16 cw97_frame;
extern s16 cw97_y;
extern s32 taiiku_course_no;

void taiiku_tama_girl1_event_step()
{
    s16 p[2];
    s32 x;
    u32 n;

    if (taiiku_tama_gclk == 600) {
        device_cmd_send(2050);
        taiiku_tama_ghold = 1;
        taiiku_tama_gpush[0] = 0;
        taiiku_tama_gpush[1] = 0;
        taiiku_tama_gcue = 0;
        cw97_frame = 127;
    } else if (taiiku_tama_gclk >= 721) {
        taiiku_tama_ghold = 0;
        taiiku_tama_gclk = 0;
    } else if (taiiku_tama_gclk >= 601) {
        if (cw97_y < 48) {
            cw97_y++;
        }
    } else {
        bg_layer_scroll(0x10000);
        camera_clamp();
        if ((taiiku_tama_gclk & 8) == 0) {
            taiiku_cw97_steer_set(0x8000);
        }
        if ((taiiku_tama_gclk & 0xF) == 0) {
            x = taiiku_tama_sx >> 3;
            p[0] = 0;
            p[1] = x;
            p[1] += x * 80 / 48;
            if (taiiku_course_no % 3 != 0) {
                if (taiiku_tama_sx >= 0 || taiiku_tama_vel > 0) {
                    p[1] = x + 32;
                }
            }
            taiiku_tama_gdst[0] = p[1];
            taiiku_tama_gdst[1] = (taiiku_tama_sy >> 3) + 16;
        }
    }
    if (taiiku_tama_ghold == 0 && (n = taiiku_tama_gclk) != 0 && n % 45 == 0) {
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
