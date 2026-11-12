/* Address: 0x8013D100 */
/* Size: 192 bytes (48 instructions) */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @PostProc: swap_instructions --offset1 0xb0 --offset2 0xb4 */
/* @PostProc: patch_insn_reg_field 0xa0:rt:a0:t8 0xa8:rs:a0:t8 0xa8:rt:a0:t8 0xb0:rs:a0:t8 0xb0:rt:t1:a0 0xb4:rs:a0:t8 0xb4:rt:t8:t1 0xb8:rs:t1:a0 0xbc:rt:t8:t1 */
#include "types.h"

typedef struct {
    s16 x;
    s16 y;
    s16 z;
    s16 cur;
} Pos;

typedef struct {
    s16 dx;
    s16 dy;
    s16 dz;
    s16 pad0E;
} Vel;

typedef struct {
    Pos pos;
    Vel vel;
} Head;

typedef struct {
    Head h;
    u8   pad10;
    u8   f17;
    u8   f18;
    u8   f19;
} Rec;

extern Rec taco_spark2_buf[];

s32 taco_spark_spawn2(Pos pos, Vel vel)
{
    Rec *p;
    s32 idx;
    s32 cnt;
    s32 cond;

    idx = taco_spark2_buf[0].h.pos.cur % 16;
    p = &taco_spark2_buf[idx];
    p->h.pos = pos;
    p->h.vel = vel;
    p->f18 = 1;
    p->f17 = 1;
    p->f19 = 128;

    cnt = taco_spark2_buf[0].h.pos.cur;
    cond = cnt < 17;
    taco_spark2_buf[0].h.pos.cur = cnt + 1;
    if (cond == 0) {
        taco_spark2_buf[0].h.pos.cur = 0;
    }
    return idx;
}
