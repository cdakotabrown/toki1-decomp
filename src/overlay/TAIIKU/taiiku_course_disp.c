/* Address: 0x8013DF90 */
/* Size: 1092 bytes (273 insns) */
/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_NO_FINDINDUCT_STRIDE=1 IDO52_FORCE_WEB_COLOR=93:t5 */
/* @RodataBase: 0x80148E38 */
#include "types.h"

typedef struct {
    s32 b2b0;
    s32 b2b4;
    s32 b2b8;
    u8 pad0[232];
    s32 b3a4;
} StepVars;
extern StepVars work_miura;
extern s32 taiiku_tama_dist;
extern s16 taiiku_tama_sx;
extern s32 taiiku_tama_spd;
typedef struct {
    u8 pad0;
    u8 ccb;
    u8 pad1[22];
    s16 ce2;
    s16 ce4;
    u8 pad2[8];
    s16 cee;
    s16 cf0;
    s16 cf2;
} CamVars;
extern CamVars cw102_anim_flag;
extern s16 taiiku_tama_gnd_x0[];
extern s16 taiiku_tama_gnd_x1[];
extern s16 taiiku_tama_gnd_x2[];
extern s16 taiiku_tama_gnd_x3[];
extern s32 taiiku_tama_push[];

void taiiku_course_disp()
{
    s32 s0;
    s32 v0, w;
    s32 i, j, k;
    s16 *p;

    switch (work_miura.b3a4) {
    case 0:
    case 3:
    case 6:
        s0 = taiiku_tama_dist;
        taiiku_tama_dist = s0 >> 1;
        taiiku_ladder_disp();
        p = (s16 *)&taiiku_tama_dist;
        v0 = p[1];
        w = v0;
        work_miura.b2b4 = v0 - 256;
        if (work_miura.b3a4 == 6) {
            cw102_anim_flag.ce4 = cw102_anim_flag.ce2 = (v0 << 4) * 3 + 4096;
            cw102_anim_flag.cee = -(taiiku_tama_sx >> 2) - (v0 >> 1) - 40;
            cw102_anim_flag.cf2 = -(v0 >> 4) - (v0 >> 1) - 104;
            if (w >= 256) {
                taiiku_tama_push[1] = 0;
                taiiku_tama_spd = 0;
                work_miura.b3a4 = 0;
                sub_step_up();
            }
        }
        taiiku_tama_dist = s0;
        if (work_miura.b2b4 >= 256) {
            for (i = 0; i < 20; i++) {
                k = i + 1;
                for (j = 0; j < 4; j++) {
                    taiiku_tama_gnd_x0[i * 4 + j] = j * (i * 4 + 80) - 160;
                    taiiku_tama_gnd_x1[i * 4 + j] = (j + 1) * (i * 4 + 80) - 160;
                    taiiku_tama_gnd_x2[i * 4 + j] = j * (k * 4 + 80) - 160;
                    taiiku_tama_gnd_x3[i * 4 + j] = (j + 1) * (k * 4 + 80) - 160;
                }
            }
            p[1] -= 512;
            work_miura.b3a4++;
        }
        break;
    case 1:
    case 4:
        taiiku_spoke_disp();
        p = (s16 *)&taiiku_tama_dist;
        work_miura.b2b8 = -p[1] * 4 + 2048;
        if (work_miura.b2b8 < -1023) {
            if (work_miura.b3a4 == 4) {
                cw102_anim_flag.cee = 176;
                cw102_anim_flag.ccb |= 0x80;
            }
            work_miura.b3a4++;
        }
        break;
    case 2:
    case 5:
        taiiku_sweep_disp();
        p = (s16 *)&taiiku_tama_dist;
        work_miura.b2b8 = -p[1] * 4 + 2048;
        if (work_miura.b2b8 < -2047) {
            for (i = 0; i < 20; i++) {
                k = i + 1;
                for (j = 0; j < 4; j++) {
                    taiiku_tama_gnd_x0[i * 4 + j] = -160 - i * 8 + (i * 4 + 80) * j;
                    taiiku_tama_gnd_x1[i * 4 + j] = -80 - i * 4 + (i * 4 + 80) * j;
                    taiiku_tama_gnd_x2[i * 4 + j] = -168 - i * 8 + (k * 4 + 80) * j;
                    taiiku_tama_gnd_x3[i * 4 + j] = -84 - i * 4 + (k * 4 + 80) * j;
                }
            }
            work_miura.b2b0 = 280;
            work_miura.b2b4 = -256;
            work_miura.b2b8 = 0;
            taiiku_tama_dist = 0;
            work_miura.b3a4++;
        }
        break;
    }
}
