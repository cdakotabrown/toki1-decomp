/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=all IDO52_AS1_KEEP_ANDI=all */
/* @SymbolStart: 0x8014F7F0 */
/* @SymbolSize: 400 */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x64 */
/* @PostProc: patch_insn_reg_field 0xf8:rt:v0:v1 0x100:rt:t9:t8 0x104:rs:v0:v1 0x104:rt:v0:v1 0x10c:rs:v0:v1 0x10c:rt:v0:t9 0x110:rs:v0:t9 0x110:rt:v1:v0 0x114:rt:v1:v0 0x11c:rt:v1:v0 0x124:rt:v0:t9 0x128:rs:t9:t8 0x128:rt:t9:t8 0x130:rs:t9:t8 0x144:rt:t8:t0 0x148:rs:t8:t0 0x148:rt:t8:t0 0x14c:rt:t0:t1 0x150:rs:t8:t0 0x158:rt:t3:t4 0x15c:rs:t0:t1 0x15c:rt:t0:t1 0x164:rs:t0:t1 0x164:rt:t1:t3 0x168:rt:t1:t3 0x17c:rs:t3:t4 0x17c:rt:t3:t4 0x184:rs:t3:t4 0x184:rt:t4:t2 0x188:rt:t4:t2 */
#include "types.h"
#include "tokimeki/libgs_types.h"

extern volatile s32 step_counter;
extern u8 back_clear_r;
extern u8 back_clear_g;
extern u8 back_clear_b;
extern u8 taco_coord_1_flg[];
extern u8 *taco_actor_rec;
extern u8 taco_round_no;
extern LIBGS_COORD2 taco_coord[];

extern s32 rand(void);
extern void taco_spark_group_step(s32);
extern void goto_sub_step(s32);

void taco_drift_step(void)
{
    s32 i;
    s32 v;
    u8 *s;
    u8 *d;
    u8 n;

    if (step_counter == 1) {
        i = 0;
        do {
            v = rand();
            *(s16 *)(taco_actor_rec + i + 3792) = (s16)((v % 8) + 5);
            i += 136;
        } while (i != 4488);
    }
    s = taco_actor_rec;
    d = taco_coord_1_flg;
    taco_coord[1].coord.t[1] += *(s16 *)(s + 3792);
    s += 136;
    for (i = 0; i < 32; i++) {
        *(s32 *)(taco_coord_1_flg + i * 80 + 108) +=
            *(s16 *)(s + i * 136 + 3792);
    }
    if (back_clear_r != 0) {
        v = back_clear_b - 1;
        back_clear_g = v & 0xFF;
        back_clear_r = v & 0xFF;
        back_clear_b = v;
    }
    if ((u32)step_counter < 401U) {
        taco_spark_group_step(17);
    }
    if ((u32)step_counter >= 600U) {
        taco_round_no = taco_round_no + 1;
        goto_sub_step(45);
    } else {
        step_counter = step_counter + 1;
    }
}
