/* @CompileEnv: IDO52_FORCE_WEB_COLOR=48:s4,49:s3 */
/* @SymbolSize: 560 */
/* @PostProc: patch_insn_reg_field 0xdc:rt:t4:t6 0xe0:rt:t4:t6 0xe8:rt:t4:t6 0xec:rt:t4:t6 0xfc:rt:t5:t7 0x100:rs:t5:t7 0x100:rt:t5:t7 0x108:rt:t5:t7 0x120:rt:t7:t9 0x124:rt:t7:t9 0x12c:rt:t7:t9 0x130:rt:t7:t9 0x148:rt:t7:t1 0x14c:rt:t7:t1 0x154:rt:t7:t1 0x158:rt:t7:t1 0x178:rt:t9:t3 0x17c:rs:t9:t3 0x17c:rt:t9:t3 0x180:rt:t8:t2 0x184:rd:t0:t4 0x184:rs:t9:t3 0x18c:rs:t0:t4 0x18c:rt:t8:t2 0x194:rt:t2:t6 0x198:rs:t2:t6 0x198:rt:t2:t6 0x1c0:rd:t3:t7 0x1c0:rs:t2:t6 0x1c4:rd:t1:t5 0x1c8:rs:t3:t7 0x1c8:rt:t1:t5 0x1d8:rt:t5:t9 0x1dc:rs:t5:t9 0x1dc:rt:t5:t9 0x204:rd:t6:t0 0x204:rs:t5:t9 0x208:rd:t4:t8 0x20c:rs:t6:t0 0x20c:rt:t4:t8 */
/* Address: 0x8013CB04, Size: 560 bytes (140 insns) */
#include "types.h"

typedef struct {
    u8 b[16];
} Blk16;

extern s32 frame_counter;
extern u8 *volatile taco_actor_rec;

extern s32 rand(void);
extern void sd_cmd_limit_send(s32, s32);
extern void taco_spark_spawn2(Blk16, s32);

s32 taco_record_effect_tick(void)
{
    u8 *p;
    s32 n;
    s32 hundred;
    s32 v;

    hundred = 100;

    for (n = 16; n != 64; n++) {
        p = n * 136 + taco_actor_rec;
        if (p[133] != 0) {
            continue;
        }
        if ((p[1] & 0x80) == 0) {
            continue;
        }
        if ((frame_counter & 0x1F) == 0) {
            sd_cmd_limit_send(1, 1288);
            p = n * 136 + taco_actor_rec;
        }
        taco_spark_spawn2(*(Blk16 *)(p + 116), 0);
        p = n * 136 + taco_actor_rec;
        taco_spark_spawn2(*(Blk16 *)(p + 116), 0);

        if (n != 8) {
            continue;
        }

        v = rand();
        (taco_actor_rec + n * 136)[133] = (u8)(v & 0x3F);

        v = rand();
        *(s16 *)(taco_actor_rec + n * 136 + 116) = (s16)(v % hundred);

        v = rand();
        *(s16 *)(taco_actor_rec + n * 136 + 118) = (s16)(v % hundred);
    }
}
