/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=all */
/* @PostProc: patch_insn_reg_field 0xa4:rt:v1:v0 0xb4:rs:v1:v0 0xc4:rs:v1:v0 0xdc:rt:t0:t1 0xe0:rs:t0:t1 0xe0:rt:t0:t1 0xec:rs:v1:v0 0xec:rt:v0:t0 0xf4:rd:t1:t2 0xf4:rt:t0:t1 0xf8:rs:t1:t2 0xf8:rt:t2:t3 0xfc:rt:t2:t3 0x100:rt:v0:t0 0x104:rt:v0:t4 0x108:rs:v0:t4 0x108:rt:v0:t4 0x110:rs:v0:t4 0x118:rs:v0:t4 0x11c:rs:v0:t4 0x120:rd:t3:t5 0x124:rs:t3:t5 0x130:rt:t4:t7 0x134:rs:t4:t7 0x134:rt:t4:t7 0x138:rt:t5:t6 0x13c:rs:t4:t7 0x140:rt:t6:t0 0x148:rt:t5:t6 0x154:rt:v0:t8 0x158:rs:v0:t8 0x158:rt:v0:t8 0x160:rs:v0:t8 0x160:rt:t7:t9 0x168:rt:t7:t9 0x16c:rs:t6:t0 0x16c:rt:t6:t0 0x174:rs:t6:t0 0x188:rt:v0:t1 0x18c:rs:v0:t1 0x18c:rt:v0:t1 0x194:rs:v0:t1 0x194:rt:t8:t2 0x19c:rt:t8:t2 */
/* @CompileFlags: -Wc,-pic0 */
/* @SymbolStart: 0x8013C584 */
/* @SymbolSize: 424 */
/* Address: 0x8013C58C  Size: 416 bytes (104 insns) */
#include "types.h"

extern volatile u16 taiiku_tama_gcue;
extern volatile u16 taiiku_tama_gclk;
extern s16 taiiku_tama_sx;
extern s16 taiiku_tama_sy;
extern s32 taiiku_tama_vel[];
struct { s16 x; s16 y; } taiiku_tama_gdst = {0, 0};

extern void device_cmd_send(s32);
extern void taiiku_cw97_steer_set(s32);
extern void bg_layer_scroll(s32);
extern void camera_clamp(void);

void taiiku_tama_girl10_event_step(void)
{
    s32 sp2C;
    s16 *p;
    s32 tmp;
    u32 temp;
    s32 x;

    p = (s16 *)&sp2C;

    if (taiiku_tama_vel[1] == 0) {
        if (taiiku_tama_gcue != 2) {
            taiiku_tama_gcue = 2;
            taiiku_tama_gclk = 0;
            device_cmd_send(2050);
        }
    }

    if ((u32)taiiku_tama_gclk >= 601) {
        taiiku_tama_gclk = 0;
    }

    bg_layer_scroll(0x18800);
    camera_clamp();

    if ((taiiku_tama_gclk & 4) == 0) {
        taiiku_cw97_steer_set(0x10000);
    }

    if ((taiiku_tama_gclk & 0xC) == 0) {
        p[0] = 0;
        p[1] = (s16)(taiiku_tama_sx >> 3);
        sp2C += taiiku_tama_vel[0] * 80;
        x = p[1];
        taiiku_tama_gdst.y = (s16)((taiiku_tama_sy >> 3) + 16);
        taiiku_tama_gdst.x = x;
    }

    temp = taiiku_tama_gclk;
    if (temp == 0) {
        return;
    }
    if ((temp % 30) != 0) {
        return;
    }
    if (temp < 60) {
        return;
    }

    if (taiiku_tama_gcue == 0) {
        taiiku_tama_gcue = 1;
        device_cmd_send(2048);
        taiiku_tama_gclk++;
    } else if (taiiku_tama_gcue == 1) {
        taiiku_tama_gcue = 0;
        device_cmd_send(2049);
        taiiku_tama_gclk++;
    } else {
        taiiku_tama_gcue = 0;
    }
}
