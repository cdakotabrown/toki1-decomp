/* @SymbolSize: 384 */
/* @CompileEnv: IDO52_AS1_NO_LA_PULL=0 IDO52_FORCE_WEB_COLOR=2:a0 */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x1C */
/* @PostProc: swap_instructions --offset1 0x7C --offset2 0x84 */
/* @PostProc: swap_instructions --offset1 0x80 --offset2 0x84 */
/* @PostProc: swap_instructions --offset1 0x84 --offset2 0x88 */
/* @PostProc: swap_instructions --offset1 0x120 --offset2 0x128 */
/* @PostProc: swap_instructions --offset1 0x124 --offset2 0x128 */
/* @PostProc: swap_instructions --offset1 0x128 --offset2 0x12C */
/* @PostProc: patch_insn_reg_field 0x14:rt:a0:v1 0x18:rs:a0:v1 0x28:rt:a0:v1 0x2c:rs:a0:v1 0x2c:rt:a0:v1 0x30:rt:v1:a0 0x34:rs:v1:a0 0x34:rt:v1:a0 0x38:rs:a0:v1 0x3c:rs:v1:a0 0x48:rt:a0:v1 0x54:rt:a0:v1 0x58:rs:a0:v1 0x68:rt:t0:t1 0x6c:rs:t0:t1 0x74:rt:t1:t2 0x78:rs:t1:t2 0x78:rt:t1:t2 0x80:rt:t2:t3 0x84:rt:t1:t2 0x88:rs:t2:t3 0x88:rt:t2:t3 0x90:rs:t2:t3 0x90:rt:t3:t4 0x98:rt:t3:t4 0x9c:rt:t4:t5 0xa0:rs:t4:t5 0xa0:rt:t4:t5 0xa8:rs:t4:t5 0xa8:rt:t5:t6 0xac:rt:t5:t6 0xd4:rt:a0:v1 0xd8:rs:a0:v1 0xd8:rt:a0:v1 0xdc:rt:t9:t0 0xe0:rs:a0:v1 0xe0:rt:t6:t8 0xe4:rs:t6:t8 0xe4:rt:v1:a0 0xe8:rd:t8:t7 0xe8:rt:a0:v1 0xec:rs:t9:t0 0xec:rt:t9:t0 0xf0:rd:t7:t9 0xf0:rt:t8:t7 0xf4:rs:t7:t9 0xf4:rt:t9:t0 0xf8:rt:a0:v1 0xfc:rs:a0:v1 0x10c:rt:t0:t2 0x110:rs:t0:t2 0x118:rt:t1:t3 0x11c:rs:t1:t3 0x11c:rt:t1:t3 0x124:rt:t2:t4 0x128:rt:t1:t3 0x12c:rs:t2:t4 0x12c:rt:t2:t4 0x134:rs:t2:t4 0x134:rt:t3:t5 0x13c:rt:t3:t5 0x140:rt:t4:t6 0x144:rs:t4:t6 0x144:rt:t4:t6 0x14c:rs:t4:t6 0x14c:rt:t5:t8 0x150:rt:t5:t8 */
/* shugaku_birth_judge3 - SHUGAKU overlay, Address: 0x80138878, Size: 384 bytes */
#include "types.h"

extern s32  get_g_zyotai_s(s32 a0);
extern void goto_sub_step(s32 a0);
extern void sub_sub_step_up(void);

extern u8  now_girl;
extern s32 girl0_birth;
extern u8  now_month;
extern u8  now_day;
extern u8  now_sub_step;
extern u8  now_sub_sub_step;
extern u8  ret_sub_step;
extern u8  ret_sub_sub_step;
extern u8  girl_no;
extern s32 p_birthday;

void shugaku_birth_judge3(void)
{
    s32 v0;
    s32 v1;

    v0 = get_g_zyotai_s((s32)now_girl);
    v1 = girl0_birth;
    if ((v1 & 0xF) == now_month &&
        (s32)(((u32)(v1 << 23)) >> 27) == now_day) {
        v1 = (s32)now_girl;
        if (v1 == 4 && (u32)(v0 & 0x7F) < 2U) {
            ret_sub_step = now_sub_step;
            ret_sub_sub_step = (u8)(now_sub_sub_step + 1);
        } else {
            ret_sub_step = (u8)(now_sub_step + 1);
            ret_sub_sub_step = 0;
        }
        girl_no = (u8)v1;
        now_girl = 0;
        goto_sub_step(6);
        return;
    }
    v1 = p_birthday;
    if (now_month == (v1 & 0xF) &&
        now_day == (s32)(((u32)(v1 << 23)) >> 27)) {
        v1 = (s32)now_girl;
        if (v1 == 4 && (u32)(v0 & 0x7F) < 2U) {
            ret_sub_step = now_sub_step;
            ret_sub_sub_step = (u8)(now_sub_sub_step + 1);
        } else {
            ret_sub_step = (u8)(now_sub_step + 1);
            ret_sub_sub_step = 0;
        }
        girl_no = (u8)v1;
        now_girl = (u8)v1;
        goto_sub_step(7);
        return;
    }
    sub_sub_step_up();
}
