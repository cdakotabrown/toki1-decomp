/* @SymbolStart: 0x80139CF8 */
/* @SymbolSize: 812 */
/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_NO_FINDINDUCT_STRIDE_OCC=bb:15,bb:18 */
/* Address: 0x80139CF8 (absorbed head 0x80139CF8, former symbol func_80139CF8) */
/* Size: 812 bytes (203 insns) */
#include "types.h"
extern s32 taiiku_course_no;
s32 taiiku_tama_girl = 0;
s32 taiiku_tama_zone = 0;
s32 taiiku_tama_btn = 0;
s32 taiiku_tama_pstep = 0;
s32 taiiku_tama_gstep[2] = { 0, 0 };
s16 taiiku_tama_ang[4] = { 0, 0, 0, 0 };
s32 taiiku_tama_spd = 0;
s32 taiiku_tama_gscrl = 0;
s32 taiiku_tama_push[2] = { 0, 0 };
s32 taiiku_tama_push_tick = 0;
s32 taiiku_tama_vel[2] = { 0, 0 };
s32 taiiku_tama_xf = 0;
s16 taiiku_tama_yf[4] = { 0, 0, 0, 0 };
struct { s32 a; s16 b; s16 c; } taiiku_tama_pfrm = { 0, 0, 0 };
s32 taiiku_tama_fall[3] = { 0, 0, 0 };
s32 taiiku_tama_gfrm = 0;
struct { s16 a, b, c, d; s32 e, f; } taiiku_tama_gfrm_tick = { 0, 0, 0, 0, 0, 0 };
s16 taiiku_tama_ghold[2] = { 0, 0 };
extern s32 taiiku_tama_dist;
extern u8 taiiku_tama_ctick;
extern s16 taiiku_tama_spd_tick;

extern s32 vblnk_counter;
extern s32 work_miura;
extern s32 taiiku_ofs_y;
extern s32 taiiku_rot;
extern s32 taiiku_tama_cpu_pos[];
extern s16 taiiku_tama_cpu_tmr[];
extern u8  taiiku_tama_cpu_stop[];
extern u8  taiiku_tama_rank[];
extern s16 taiiku_tama_gnd_y0[];
extern s16 taiiku_tama_gnd_y1[];
extern s16 taiiku_tama_gnd_x0[];
extern s16 taiiku_tama_gnd_x1[];
extern s16 taiiku_tama_gnd_x2[];
extern s16 taiiku_tama_gnd_x3[];

taiiku_race_init2()
{
    s32 i;
    s32 j;

    taiiku_tama_gscrl = 0;
    taiiku_tama_spd = 0;
    taiiku_tama_yf[2] = 0;
    taiiku_tama_yf[3] = 256;
    taiiku_tama_xf = 0;
    taiiku_tama_yf[1] = 128;
    taiiku_tama_yf[0] = 0;
    taiiku_tama_push[0] = 0;
    taiiku_tama_push[1] = 0;
    taiiku_tama_vel[0] = 0;
    taiiku_tama_vel[1] = 0;
    taiiku_tama_girl = 0;
    taiiku_tama_btn = 0;
    taiiku_tama_pstep = 0;
    taiiku_tama_dist = 0;
    taiiku_course_no = 0;
    taiiku_tama_zone = 1;
    taiiku_tama_spd_tick = 0;
    taiiku_tama_ctick = 0;
    taiiku_tama_pfrm.a = 0;
    taiiku_tama_pfrm.b = 0;
    taiiku_tama_fall[1] = 0;
    taiiku_tama_fall[2] = 0;
    taiiku_tama_fall[0] = 1;
    taiiku_tama_pfrm.c = 0;

    taiiku_tama_gfrm = 0;
    taiiku_tama_gfrm_tick.a = 0;
    taiiku_tama_gfrm_tick.e = 0;
    taiiku_tama_gfrm_tick.f = 0;
    taiiku_tama_gfrm_tick.b = 0;
    taiiku_tama_gfrm_tick.c = -32;
    taiiku_tama_gfrm_tick.d = 48;
    taiiku_tama_ghold[0] = 0;
    taiiku_tama_ghold[1] = 0;
    srand(vblnk_counter);
    taiiku_tama_ang[0] = 0;
    taiiku_tama_ang[1] = 0;
    taiiku_tama_ang[2] = 0;
    work_miura = 280;
    taiiku_ofs_y = -256;
    taiiku_rot = 0;
    for (i = 0; i < 3; i++) {
        taiiku_tama_cpu_stop[i] = 0;
        taiiku_tama_cpu_tmr[i] = 0;
        taiiku_tama_cpu_pos[i] = 0;
    }
    for (i = 0; i < 4; i++) {
        taiiku_tama_rank[i] = 255;
    }
    for (i = 0; i < 80; i++) {
        taiiku_tama_gnd_y0[i] = (i >> 2) * 8 - 24;
        taiiku_tama_gnd_y1[i] = (i >> 2) * 8 - 16;
    }
    for (i = 0; i < 20; i++) {
        for (j = 0; j < 4; j++) {
            taiiku_tama_gnd_x0[i * 4 + j] = -160 - i * 8 + (i * 4 + 80) * j;
            taiiku_tama_gnd_x1[i * 4 + j] = -80 - i * 4 + (i * 4 + 80) * j;
            taiiku_tama_gnd_x2[i * 4 + j] = -168 - i * 8 + (((i + 1) << 2) + 80) * j;
            taiiku_tama_gnd_x3[i * 4 + j] = -84 - i * 4 + (((i + 1) << 2) + 80) * j;
        }
    }
}
