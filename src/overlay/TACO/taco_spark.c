/* @PostProc: scheduler_normalizer --const-fp --store-fp --branch-fp --load-fp */
/* @PostProc: patch_insn_reg_field 0x64:rt:t8:v1 0x68:rs:t8:v1 0x68:rt:t9:v1 0x70:rs:t9:v1 0x80:rt:t1:v1 0x84:rs:t1:v1 0x84:rt:t2:v1 0x88:rt:t0:t8 0x8c:rs:t2:v1 0x90:rt:t0:t8 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:6 */
#include "types.h"

typedef struct {
    s16 x;
    s16 y;
    s16 z;
    s8  pad06[0x11 - 0x06];
    u8  f17;
    u8  f18;
    u8  f19;
} Rec;

extern Rec taco_spark_buf[16];
extern Rec taco_spark2_buf;

extern void taco_spark_disp(s32 x, s32 y, s32 z, s32 c);

s32 taco_spark(void)
{
    Rec *p;
    Rec *end;
    s32 v0;
    s32 v1;

    end = &taco_spark2_buf;
    p = &taco_spark_buf[0];
    v0 = 0;
    do {
        if (p->f19 != 0) {
            taco_spark_disp((s16)p->x, (s16)p->y, (s16)p->z,
                          (u8)p->f17 - 1);
            v0 = (u8)p->f18;
            v1 = ((u32)v0 < 5U) ^ 1;
            p->f18 = (u8)(v0 + 1);
            if (v1 != 0) {
                v0 = (u8)p->f17;
                p->f18 = 0;
                v1 = ((u32)v0 < 7U) ^ 1;
                p->f17 = (u8)(v0 + 1);
                if (v1 != 0) {
                    p->f19 = 0;
                    p->f17 = 0;
                }
            }
        }
        p++;
    } while (p != end);
    return v0;
}
