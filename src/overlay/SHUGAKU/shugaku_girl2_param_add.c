/* Address: 0x80135D68 (true entry 0x80135D60 -- cross-prologue head) */
/* @SymbolStart: 0x80135D60 */
/* @SymbolSize: 236 */
/* @PostProc: patch_insn_reg_field 0x24:rt:v1:t7 0x28:rs:v1:t7 0x28:rt:v1:t7 0x2c:rt:a1:a0 0x30:rs:v1:t7 0x30:rt:a1:a0 0x34:rt:a0:v1 0x38:rs:a0:v1 0x38:rt:a0:v1 0x3c:rt:v1:t1 0x40:rd:t7:t8 0x44:rs:a0:v1 0x44:rt:t7:t8 0x48:rt:t8:t9 0x50:rs:t8:t9 0x50:rt:t9:t0 0x54:rt:t9:t0 0x58:rs:v1:t1 0x58:rt:v1:t1 0x60:rs:v1:t1 0x60:rt:a1:a0 0x64:rd:t0:t2 0x68:rs:a0:v1 0x68:rt:t0:t2 0x6c:rt:t1:t3 0x74:rs:t1:t3 0x74:rt:t2:t4 0x7c:rt:t2:t4 0x88:rt:v1:t5 0x8c:rs:v1:t5 0x8c:rt:v1:t5 0x90:rt:a1:a0 0x94:rs:v1:t5 0x94:rt:a1:a0 0x98:rt:a0:v1 0x9c:rs:a0:v1 0x9c:rt:a0:v1 0xa0:rd:t3:t6 0xa4:rs:a0:v1 0xa4:rt:t3:t6 0xa8:rt:t4:t7 0xb0:rs:t4:t7 0xb0:rt:t5:t8 0xb8:rt:t5:t8 0xbc:rt:t6:t9 0xc0:rs:t6:t9 0xc0:rt:t6:t9 0xc4:rt:t8:t1 0xc8:rs:t8:t1 0xc8:rt:t8:t1 0xd0:rs:t6:t9 0xd0:rt:t7:t0 0xd4:rt:t7:t0 0xdc:rs:t8:t1 0xdc:rt:t9:t2 0xe0:rt:t9:t2 */
#include "types.h"

typedef struct {
    u8 b[56];
} Row;

extern u8 shugaku_rpg_arg;
extern volatile u8 now_girl;
extern u8 now_sub_sub_step;
extern volatile s16 with_yoshi;
extern Row screen_w[];

extern void param_clamp(void);
extern void sub_sub_step_up(void);

void shugaku_girl2_param_add(void) {
    u8 *p;
    u8 idx;
    s32 t6;

    t6 = shugaku_rpg_arg;
    if (t6 == 0) {
        idx = now_girl;
        p = (u8 *)&screen_w[idx];
        *(s16 *)(p + 446) = (s16)(*(s16 *)(p + 446) + 1);
        idx = now_girl;
        p = (u8 *)&screen_w[idx];
        *(s16 *)(p + 450) = (s16)(*(s16 *)(p + 450) + 2);
        param_clamp();
    } else {
        idx = now_girl;
        p = (u8 *)&screen_w[idx];
        *(s16 *)(p + 450) = (s16)(*(s16 *)(p + 450) + 1);
        param_clamp();
        with_yoshi = (s16)(with_yoshi + 2);
        now_sub_sub_step = (u8)(now_sub_sub_step + 11);
    }
    sub_sub_step_up();
}
