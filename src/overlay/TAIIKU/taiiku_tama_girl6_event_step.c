/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=all */
/* @SymbolStart: 0x8013BD84 */
/* @SymbolSize: 432 */
/* @PostProc: patch_stack_complete --old 40 --new 48 */
/* @PostProc: remap_stack_slots --map '36:44' */
/* @PostProc: patch_insn_reg_field 0xac:rt:v1:v0 0xbc:rs:v1:v0 0xcc:rs:v1:v0 0xe4:rt:t1:t2 0xe8:rs:t1:t2 0xe8:rt:t1:t2 0xf4:rs:v1:v0 0xf4:rt:v0:t1 0xfc:rd:t2:t3 0xfc:rt:t1:t2 0x100:rs:t2:t3 0x100:rt:t3:t4 0x104:rt:t3:t4 0x108:rt:v0:t1 0x10c:rt:v0:t5 0x110:rs:v0:t5 0x110:rt:v0:t5 0x118:rs:v0:t5 0x120:rs:v0:t5 0x124:rs:v0:t5 0x128:rd:t4:t6 0x12c:rs:t4:t6 0x138:rt:t5:t8 0x13c:rs:t5:t8 0x13c:rt:t5:t8 0x140:rt:t6:t7 0x144:rs:t5:t8 0x148:rt:t9:t1 0x150:rt:t6:t7 0x15c:rt:t8:t9 0x160:rs:t8:t9 0x160:rt:t8:t9 0x168:rs:t8:t9 0x168:rt:t7:t0 0x170:rt:t7:t0 0x174:rs:t9:t1 0x174:rt:t9:t1 0x17c:rs:t9:t1 0x190:rt:t0:t2 0x194:rs:t0:t2 0x194:rt:t0:t2 0x19c:rs:t0:t2 0x19c:rt:t1:t3 0x1a4:rt:t1:t3 */
/* Address: 0x8013BD8C (true entry 0x8013BD84 -- the hoisted taiiku_tama_vel_1 head */
/* Size: 432 bytes (108 insns) including the absorbed head */
#include "types.h"

extern volatile u16 taiiku_tama_gclk;
extern volatile u16 taiiku_tama_gcue;
extern s16 taiiku_tama_sx;
extern s16 taiiku_tama_sy;
struct { s16 x; s16 y; } taiiku_tama_gdst = {0, 0};
extern s32 taiiku_tama_vel[];

extern void device_cmd_send(s32);
extern void taiiku_cw97_steer_set(s32);
extern void bg_layer_scroll(s32);
extern void camera_clamp(void);

typedef union {
    struct {
        s16 a;
        s16 b;
    } h;
    s32 w;
} Pair;

void taiiku_tama_girl6_event_step(void)
{
    Pair loc;
    s16 *p;
    u32 n;
    s32 x;

    if (taiiku_tama_vel[1] == 0) {
        if (taiiku_tama_gcue != 2) {
            device_cmd_send(2050);
            taiiku_tama_gcue = 2;
            taiiku_tama_gclk = 0;
        }
    }
    if (taiiku_tama_gclk >= 601U) {
        taiiku_tama_gclk = 31;
    }
    bg_layer_scroll(0x1C000);
    camera_clamp();
    if ((taiiku_tama_gclk & 4) == 0) {
        taiiku_cw97_steer_set(0x12000);
    }
    if ((taiiku_tama_gclk & 0xF) == 0) {
        p = (s16 *)&loc;
        p[0] = 0;
        p[1] = taiiku_tama_sx >> 3;
        loc.w += taiiku_tama_vel[0] * 80;
        x = p[1];
        taiiku_tama_gdst.y = (taiiku_tama_sy >> 3) + 16;
        taiiku_tama_gdst.x = x;
    }
    n = taiiku_tama_gclk;
    if (n == 0) {
        return;
    }
    if ((n % 30) != 0) {
        return;
    }
    if (n < 60U) {
        return;
    }
    if (taiiku_tama_gcue == 0) {
        taiiku_tama_gcue = 1;
        device_cmd_send(2048);
        taiiku_tama_gclk = taiiku_tama_gclk + 1;
        return;
    }
    if (taiiku_tama_gcue == 1) {
        taiiku_tama_gcue = 0;
        device_cmd_send(2049);
        taiiku_tama_gclk = taiiku_tama_gclk + 1;
        return;
    }
    taiiku_tama_gcue = 0;
}
