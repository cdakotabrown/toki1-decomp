/* @PostProc: patch_insn_reg_field 0x158:rt:t6:t7 0x15c:rs:t6:t7 0x15c:rt:t6:t7 0x160:rt:t9:t6 0x164:rd:t7:t8 0x164:rs:s0:t7 0x164:rt:t6:s0 0x168:rs:t7:t8 0x168:rt:t8:t9 0x16c:rs:t6:t7 0x170:rs:t8:t6 0x180:rt:t3:t4 0x184:rs:t3:t4 0x184:rt:t3:t4 0x188:rt:t7:t3 0x18c:rd:t4:t5 0x18c:rs:s0:t4 0x18c:rt:t3:s0 0x190:rs:t4:t5 0x190:rt:t5:t8 0x194:rs:t3:t4 0x198:rd:t8:t6 0x198:rs:t5:t3 0x198:rt:t7:t8 0x19c:rs:t8:t6 0x1b0:rt:t6:t7 0x1b4:rs:t6:t7 0x1b8:rd:t4:t5 0x1bc:rs:t4:t5 0x1c0:rt:t5:t3 0x1c4:rs:t5:t3 0x1c4:rt:t5:t3 0x1cc:rd:t7:t8 0x1cc:rs:t5:t3 0x1d0:rs:t7:t8 0x1d4:rt:t8:t6 0x1d8:rs:t8:t6 0x1d8:rt:t8:t6 0x1e0:rs:t8:t6 0x1e0:rt:t3:t4 0x1e4:rt:t3:t4 0x200:rt:t6:t7 0x204:rt:t6:t7 */
/* Address: 0x80147E58 */
/* Size: 552 bytes (138 insns) */
#include "types.h"

extern u8 *taco_actor_rec;
extern s32 frame_counter;

extern s32 rand(void);
extern void taco_spark_disp2(s16, s16, s16, s32);

void taco_spark_group_step(s32 idx)
{
    s32 i;
    s16 r;
    s32 rv;
    u8 *b;
    u8 *p;
    u8 st;

    for (i = 0; i != 2176; i += 136) {
        st = (taco_actor_rec + i)[3539];
        if (st != 0 && (u32)st < 9U) {
            r = (s16)(rand() & 0xFF);

            p = taco_actor_rec + i;
            taco_spark_disp2(*(s16 *)(p + 3652), *(s16 *)(p + 3654),
                          *(s16 *)(p + 3656), p[3539] - 1);

            p = taco_actor_rec + i;
            taco_spark_disp2((s16)(*(s16 *)(p + 3652) + r),
                          *(s16 *)(p + 3654), *(s16 *)(p + 3656),
                          p[3539] - 2);

            p = taco_actor_rec + i;
            taco_spark_disp2((s16)(*(s16 *)(p + 3652) - r),
                          (s16)(*(s16 *)(p + 3654) + r),
                          *(s16 *)(p + 3656), p[3539] - 3);

            p = taco_actor_rec + i;
            taco_spark_disp2(*(s16 *)(p + 3652),
                          (s16)(*(s16 *)(p + 3654) - r),
                          *(s16 *)(p + 3656), p[3539] - 4);
        } else {
            rv = rand() & 0x3FF;
            b = taco_actor_rec;
            *(s16 *)(b + i + 3652) =
                (s16)(rv + *(s16 *)(b + idx * 136 + 116) - 512);
            rv = rand() & 0x3FF;
            b = taco_actor_rec;
            *(s16 *)(b + i + 3654) =
                (s16)(rv + *(s16 *)(b + idx * 136 + 118) - 512);
            rv = rand() & 0xFFF;
            b = taco_actor_rec;
            *(s16 *)(b + i + 3656) = (s16)(rv - 6000);
            taco_actor_rec[i + 3539] = 0;
        }

        if ((frame_counter & 3) == 3) {
            p = taco_actor_rec + i;
            p[3539] = p[3539] + 1;
        }
    }
}
