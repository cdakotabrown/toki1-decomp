/* @SymbolSize: 928 */
/* Address: 0x801370A8 */
/* Size: 928 bytes (232 insns) */
/* raw-address-cast paraphrase carrying `@CompileFlags: -Wc,-pic0`, */
/* `@SymbolSize: 952` that ran 24 bytes past the real `jr $ra` -- all */
/* 1. @SymbolSize: 928 -- the .sym entry (952) over-runs the real `jr $ra` */
/* @CompileEnv: IDO52_CONST_INREG_VALUES=3:-1:2:10 */
/* @PostProc: patch_insn_reg_field 0x15C:rt:t3:t2 0x140:rs:t8:s4 0x140:rt:s4:t8 0x144:rt:t0:t9 0x148:rs:t0:t9 0x14c:rd:t1:t0 0x150:rd:t2:t1 0x150:rt:t1:t0 0x154:rd:t3:t2 0x154:rs:t2:t1 0x168:rt:t5:t4 0x174:rt:t2:t0 0x178:rs:t5:t4 0x178:rt:t5:t4 0x17c:rt:t4:t3 0x180:rd:t6:t5 0x180:rs:t5:s4 0x180:rt:s4:t4 0x184:rs:t6:t5 0x184:rt:t8:t5 0x188:rs:t8:t5 0x190:rd:t9:t6 0x194:rd:t0:t8 0x194:rs:t4:t3 0x194:rt:t9:t6 0x198:rd:t7:t9 0x198:rs:t0:t8 0x1a0:rt:t7:t9 0x1a4:rs:t2:t0 0x1a4:rt:t2:t0 0x1a8:rt:t1:t7 0x1ac:rd:t3:t1 0x1ac:rs:t2:s4 0x1ac:rt:s4:t0 0x1b0:rs:t3:t1 0x1b0:rt:t5:t1 0x1b4:rd:t6:t2 0x1b4:rt:t5:t1 0x1bc:rd:t8:t4 0x1bc:rs:t1:t7 0x1bc:rt:t6:t2 0x1c0:rd:t4:t5 0x1c0:rs:t8:t4 0x1c4:rt:t4:t5 0x1c8:rt:t9:t3 0x1d0:rs:t9:t3 0x1ec:rt:t2:t9 0x1f0:rt:t2:t9 0x1f4:rd:t0:t6 0x1f8:rs:t0:t6 0x1f8:rt:t7:t8 0x200:rt:t7:t8 0x22c:rd:t3:t0 0x230:rt:t3:t0 0x23c:rd:t5:t1 0x240:rt:t5:t1 0x250:rt:t6:t2 0x254:rs:t6:t2 0x254:rt:t6:t2 0x25c:rs:t6:t2 0x284:rt:t0:t6 0x288:rs:t0:t6 0x288:rt:t0:t6 0x28c:rd:t4:t5 0x290:rt:t4:t5 0x298:rd:t7:t8 0x298:rs:t0:s4 0x298:rt:s4:t6 0x29c:rs:t7:t8 0x29c:rt:t2:t8 0x2a0:rs:t2:t8 0x2a0:rt:t3:t9 0x2a4:rd:t9:t3 0x2a8:rs:t9:t3 0x2a8:rt:t3:t9 0x2ac:rd:t8:t4 0x2b0:rt:t8:t4 0x2bc:rt:t2:t8 0x2c4:rd:t8:t2 0x2c8:rs:t8:t2 0x2cc:rt:t2:t8 0x2d8:rd:t5:t0 0x2dc:rd:t6:t7 0x2dc:rt:t5:t0 0x2e0:rt:t6:t7 0x2f0:rs:a1:t4 0x2f0:rt:t4:a1 0x2f4:rd:t0:t5 0x2fc:rt:t0:t5 0x300:rt:t9:t3 0x304:rt:t3:t9 0x308:rs:t9:t3 0x318:rt:t3:t9 0x328:rt:t5:t0 0x330:rt:t5:t0 0x35c:rd:t6:t7 0x360:rs:t6:t7 0x360:rt:t8:t2 0x364:rt:t8:t2 */
#include "types.h"

extern s32 rand(void);
extern void line_set(s32 a0, s32 a1, s16 a2, s16 a3, s32 a4, s32 a5,
                          s32 a6);

extern s16 cal_cw_tick;
extern s16 cal_cw_anim;
extern s16 cal_cw_frame;
extern u8  cal_cw_mode;
extern u8  cal_cw_bright;

extern u8  class_w[];
extern u8  kangei_streak_pos_wk[];
extern s16 kangei_streak_spd_wk[];

void kangei_scroll(void)
{
    s16 i;
    u8 *entry;
    s16 v;
    s16 *pa;
    s16 *pb;

    if (cal_cw_anim == 0 && cal_cw_frame == 7 && cal_cw_tick == 2) {
        cal_cw_anim = 1;
        cal_cw_frame = 0;
        cal_cw_tick = 0;
        cal_cw_mode = 1;
    }
    if (cal_cw_anim == 2 && cal_cw_frame == 1 && cal_cw_tick == 4) {
        cal_cw_anim = 3;
        cal_cw_frame = 0;
        cal_cw_tick = 0;
        cal_cw_mode = 1;
    }

    i = 97;
    do {
        entry = class_w + (s32)i * 68;
        v = *(s16 *)(entry + 24);
        if (v < 2 || v >= 8) {
            *(s32 *)(entry + 40) = *(s32 *)(entry + 40)
                - 327680 * !(cal_cw_anim ^ 3) - 327680;
        } else if (v == 4 || v == 7) {
            *(s32 *)(entry + 40) = *(s32 *)(entry + 40)
                - 196608 * !(cal_cw_anim ^ 3) - 196608;
        } else {
            *(s32 *)(entry + 40) = *(s32 *)(entry + 40)
                - 131072 * !(cal_cw_anim ^ 3) - 131072;
        }
        if (*(s16 *)(entry + 42) < -159) {
            *(s16 *)(entry + 38) = (s16)(rand() % 320 - 160);
            *(s16 *)(entry + 42) = 60;
            *(s16 *)(entry + 24) = (s16)(rand() % 10);
        }
        i++;
    } while (i < 116);

    i = 0;
    if ((u32)cal_cw_bright >= 120U) {
        do {
            pb = &kangei_streak_spd_wk[i];
            pa = (s16 *)(kangei_streak_pos_wk + (s32)i * 4);
            pa[1] = (s16)(pa[1]
                          - (*pb * 4) * (!(cal_cw_anim ^ 3) + 1));
            line_set(pa[0], pa[1], pa[0],
                          (s16)(*pb * 20 + pa[1]),
                          0xFFFFFF, 0xFFFFFF, 11);
            if (pa[1] < -170) {
                pa[1] = 60;
                pa[0] = (s16)(rand() % 320 - 160);
                *pb = (s16)(rand() % 5 + 1);
            }
            i++;
        } while (i < 10);
    }
}
