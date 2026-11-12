/* Address: 0x8013D1C0 */
/* Size: 168 bytes (42 instructions) */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @PostProc: swap_instructions --offset1 0x38 --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x3c --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x94 --offset2 0x98 */
/* @PostProc: patch_insn_reg_field 0x88:rt:a3:t7 0x8c:rs:a3:t7 0x8c:rt:a3:t7 0x94:rs:a3:t7 0x94:rt:t8:a3 0x98:rs:a3:t7 0x98:rt:t7:t8 0xa0:rs:t8:a3 0xa4:rt:t7:t8 */
#include "types.h"

typedef struct {
    s16 x;
    s16 y;
    s16 z;
    s16 cur;
    s8  pad08[0x11 - 0x08];
    u8  f17;
    u8  f18;
    u8  f19;
} Rec;

extern Rec taco_spark_buf[];

s32 taco_spark_spawn(s16 x, s16 y, s16 z)
{
    Rec *p;
    s32 idx;
    s32 cnt;
    s32 cond;

    idx = taco_spark_buf[0].cur % 16;
    p = &taco_spark_buf[idx];
    p->x = x;
    p->y = y;
    p->z = z;
    p->f18 = 1;
    p->f17 = 1;
    p->f19 = 128;

    cnt = taco_spark_buf[0].cur;
    cond = cnt < 17;
    taco_spark_buf[0].cur = cnt + 1;
    if (cond == 0) {
        taco_spark_buf[0].cur = 0;
    }
    return idx;
}
