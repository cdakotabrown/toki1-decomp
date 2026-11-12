/* @SymbolStart: 0x80158F58 */
/* @SymbolSize: 328 */
/* @PostProc: patch_stack_complete --old 48 --new 56 */
/* @PostProc: remap_stack_slots --map '32:44,40:48,42:50,44:52' */
/* @PostProc: swap_instructions --offset1 0x11C --offset2 0x120 */
/* @PostProc: patch_insn_reg_field 0x30:rs:t7:t6 0x30:rt:t6:t7 0xb8:rs:t6:v1 0xb8:rt:v1:t6 0xd4:rs:t9:v1 0xd4:rt:v1:t9 0xf8:rs:t2:v1 0xf8:rt:v1:t2 0x128:rs:t3:v1 0x128:rt:v1:t3 */
/* Address: 0x80158F64 (cross-prologue head at 0x80158F58: sll $t6,$a0,4 + */
#include "types.h"

typedef struct {
    s16 x;
    s16 y;
    s16 z;
    s16 w;
} TacoVec;

extern u8 *taco_actor_rec;

extern void taco_shot_spawn(TacoVec v, s32 a2, s32 a3);
extern void taco_actor_disp_switch(s32 a0, s32 a1);

void taco_actor_step16(s32 arg0)
{
    TacoVec v;
    s32 off;

    off = arg0 * 136;
    if ((*(s16 *)(taco_actor_rec + off + 122) % 20) == 0) {
        v.x = *(s16 *)(taco_actor_rec + off + 116);
        v.y = *(s16 *)(taco_actor_rec + off + 118);
        v.z = *(s16 *)(taco_actor_rec + off + 120);
        taco_shot_spawn(v, 128, 128);
    }
    *(s16 *)(taco_actor_rec + off + 118) += 32;

    if (!(*(s16 *)(taco_actor_rec + off + 118) < 1025)) {
        *(u8 *)(taco_actor_rec + off + 2) = 254;
    }
    if (*(s16 *)(taco_actor_rec + off + 124) == 0) {
        taco_actor_disp_switch(0, arg0);
        *(s16 *)(taco_actor_rec + off + 124) += 1;
    } else {
        taco_actor_disp_switch(1, arg0);
    }
}
