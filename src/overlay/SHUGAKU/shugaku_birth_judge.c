/* @PostProc: scheduler_normalizer --block-aware */
/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x48 */
/* @PostProc: swap_instructions --offset1 0x64 --offset2 0x7c */
/* @PostProc: swap_instructions --offset1 0xcc --offset2 0xec */
/* @PostProc: patch_insn_reg_field 0x28:rt:a1:v1 0x2c:rs:a0:v1 0x2c:rt:a0:v1 0x34:rs:a1:a0 0x34:rt:v1:a0 0x38:rs:a0:v1 0x3c:rs:t6:a0 0x3c:rt:v1:t6 0x40:rt:t9:t7 0x44:rs:t9:t7 0x44:rt:t9:t7 0x48:rd:t7:t8 0x48:rt:a0:v1 0x4c:rd:t8:t9 0x4c:rt:t7:t8 0x50:rs:t8:t7 0x54:rt:at:v1 0x58:rs:t0:v1 0x58:rt:t0:v1 0x60:rs:t0:v1 0x60:rt:t1:t0 0x64:rt:t1:t0 0x70:rt:t2:t1 0x74:rs:t2:t1 0x74:rt:t2:t1 0x7c:rt:t2:t1 0x80:rt:t0:at 0x98:rt:a0:v1 0x9c:rs:a0:v1 0x9c:rt:a0:v1 0xa0:rt:t6:t5 0xa4:rs:a0:v1 0xa4:rt:t3:t2 0xa8:rs:t3:t2 0xa8:rt:v1:a0 0xac:rd:t4:t3 0xac:rt:a0:v1 0xb0:rs:t6:t5 0xb0:rt:t6:t5 0xb4:rd:t5:t4 0xb4:rt:t4:t3 0xb8:rs:t5:t4 0xb8:rt:t6:t5 0xbc:rt:at:v1 0xc0:rs:t7:v1 0xc0:rt:t7:v1 0xc8:rs:t7:v1 0xc8:rt:t8:t6 0xcc:rt:t8:t6 0xd8:rt:t9:t8 0xdc:rs:t9:t8 0xdc:rt:t9:t8 0xe0:rt:t7:at 0xec:rt:t9:t8 0xfc:rt:a1:v1 0x104:rs:a1:v1 0x108:rt:t0:t7 0x10c:rs:t0:t7 0x10c:rt:t0:t7 0x110:rt:v1:a0 0x114:rs:v1:a0 0x114:rt:t0:t7 0x118:rt:t1:t9 0x11c:rs:t1:t9 0x11c:rt:t1:t9 0x120:rs:a1:v1 0x120:rt:t2:t0 0x124:rs:v1:t9 0x124:rt:t1:a0 0x12c:rt:t2:t0 0x148:rt:v1:a0 0x14c:rs:v1:a0 0x14c:rt:a1:v1 0x150:rt:t3:t1 0x154:rs:t3:t1 0x154:rt:t3:t1 0x15c:rs:t3:t1 0x160:rt:t4:t3 0x164:rs:t4:t3 0x16c:rs:a1:v1 0x16c:rt:t5:t4 0x174:rt:t5:t4 */
/* @SymbolSize: 408 */
#include "types.h"

extern s32 get_g_zyotai_s(s32 a0);
extern void goto_sub_step(s32 a0);
extern void sub_sub_step_up(void);

extern u8  now_girl;
extern s32 girl0_birth;
extern u8  now_month;
extern u8  now_day;
extern volatile u8 now_sub_step;
extern u8  ret_sub_step;
extern u8  ret_sub_sub_step;
extern u8  girl_no;
extern s32 p_birthday;
extern u8  shugaku_place;

void shugaku_birth_judge(void)
{
    s32 v0, v1, t2, t3, t4, t5, t6, t8, t9;
    u8 t7v, t5v;
    u8 a0;
    u8 fl;

    a0 = now_girl;
    v0 = get_g_zyotai_s((s32)a0);
    v1 = girl0_birth;
    a0 = now_month;
    t6 = v1 & 0xF;
    if (a0 != t6) goto lbl_80138768;
    t7v = now_day;
    t8 = v1 << 23;
    t9 = (u32)t8 >> 27;
    if (t7v != t9) goto lbl_80138768;
    ret_sub_step = (u8)(now_sub_step + 1);
    ret_sub_sub_step = 0;
    girl_no = now_girl;
    now_girl = 0;
    goto_sub_step(6);
    return;
lbl_80138768:
    v1 = p_birthday;
    t2 = v1 & 0xF;
    if (t2 != a0) goto lbl_801387C8;
    t3 = v1 << 23;
    t5v = now_day;
    t4 = (u32)t3 >> 27;
    if (t4 != t5v) goto lbl_801387C8;
    ret_sub_step = (u8)(now_sub_step + 1);
    ret_sub_sub_step = 0;
    girl_no = now_girl;
    goto_sub_step(7);
    return;
lbl_801387C8:
    fl = now_sub_step;
    if (fl == 4 && now_girl == 2 && shugaku_place == 2) {
        ret_sub_step = (u8)(fl + 1);
        ret_sub_sub_step = 0;
        goto_sub_step(8);
    } else if (fl == 2 && now_girl == 5 && (u32)(v0 & 0x7F) < 2U) {
        ret_sub_step = (u8)(fl + 1);
        ret_sub_sub_step = 0;
        goto_sub_step(11);
    } else {
        sub_sub_step_up();
    }
}
