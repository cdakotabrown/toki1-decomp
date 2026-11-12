/* @CompileFlags: -Wc,-pic1 */
/* @SymbolSize: 432 */
/* Address: 0x8013B448 */
/* Size: 432 bytes (108 insns) */
/* @CompileEnv: IDO52_PRECOLOR_WEB=a1:0,a3:19,a2:24 IDO52_UGEN_TREG_ORDER=t6,t7,t9,t8,t0,t1,t2,t3,t4,t5 */
/* @PostProc: patch_stack_complete --old 88 --new 48 */
/* @PostProc: rename_register_chain --chain t2:t1:t3 --range 0x158:0x174 */
/* @PostProc: patch_insn_reg_field 0xa4:rs:t9:t8 0xa4:rt:t8:t9 0x10c:rt:v0:t8 0x110:rd:t8:t0 0x114:rs:t8:t0 0x114:rt:t0:t2 0x118:rt:t0:t2 0x11c:rt:v0:t8 0x178:rt:t3:t4 0x180:rt:t3:t4 0x1a8:rt:t4:t5 0x1ac:rt:t4:t5 */
#include "types.h"

extern s32 device_cmd_send();
extern s32 taiiku_cw97_steer_set();
extern s32 bg_layer_scroll();
extern s32 camera_clamp();

extern volatile u16 taiiku_tama_gclk;
extern s32 taiiku_tama_vel;
extern s16 taiiku_tama_sx;
extern s16 taiiku_tama_sy;
s16 taiiku_tama_gdst[2] = {0, 0};
extern u16 taiiku_tama_gcue;
extern s32 taiiku_course_no;

void taiiku_scroll0(void) {
    u32 a1;
    s32 a2, a3, v0, t5;
    s32 mul, d48, d3, d30, t;
    union { s16 h[2]; s32 w; } buf;

    bg_layer_scroll(0x18000);
    camera_clamp();
    a1 = taiiku_tama_gclk;
    if ((a1 & 4) == 0) {
        taiiku_cw97_steer_set(0x10000);
        a1 = taiiku_tama_gclk;
    }
    if (a1 % 6 == 0) {
        mul = 40;
        d48 = 48;
        a3 = taiiku_tama_vel;
        a2 = taiiku_tama_sx;
        v0 = a2 >> 3;
        buf.h[0] = 0;
        buf.h[1] = (s16)v0;
        buf.w = buf.w + a3 * mul;
        buf.h[1] = (s16)(buf.h[1] + (v0 * mul) / d48);
        d3 = 3;
        t5 = taiiku_course_no;
        if (t5 % d3 != 0) {
            if (a2 >= 0 || a3 > 0) {
                buf.h[1] = (s16)(v0 + 32);
            }
        }
        t = buf.h[1];
        taiiku_tama_gdst[1] = (s16)((taiiku_tama_sy >> 3) + 16);
        taiiku_tama_gdst[0] = (s16)t;
    }
    if (a1 < 570) {
        goto L574;
    }
    taiiku_tama_gcue = 0;
    device_cmd_send(2050);
    a1 = 0;
L574:
    taiiku_tama_gclk = (u16)a1;
    if (a1 == 0) {
        return;
    }
    taiiku_tama_gclk = (u16)a1;
    d30 = 30;
    if (a1 % d30 != 0) {
        return;
    }
    if (taiiku_tama_gcue == 0) {
        taiiku_tama_gcue = 1;
        device_cmd_send(2048);
    } else {
        taiiku_tama_gcue = 0;
        device_cmd_send(2049);
    }
    a1 = taiiku_tama_gclk;
    taiiku_tama_gclk = (u16)(a1 + 1);
}
