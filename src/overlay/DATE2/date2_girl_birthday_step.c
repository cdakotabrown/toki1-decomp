/* @PostProc: scheduler_normalizer --block-aware */
/* @PostProc: patch_insn_reg_field 0x14:rt:a0:a1 0x18:rs:a0:a1 0x54:rt:t1:t2 0x58:rs:t1:t2 0x60:rt:t7:v0 0x64:rt:t2:t7 0x68:rs:t2:t7 0x68:rt:t2:t7 0x70:rs:t4:v0 0x70:rt:t4:v0 0x78:rs:t2:t7 0x78:rt:t3:t9 0x7c:rt:t3:t9 0x80:rt:t4:a1 0x84:rs:t4:v0 0x84:rt:t5:t4 0x88:rs:v0:a1 0x8c:rs:t5:t4 0xb4:rs:t7:v0 0xb4:rt:t7:v0 0xbc:rs:t7:v0 0xbc:rt:t9:t0 0xc0:rt:t9:t0 */
/* @SymbolSize: 204 */
/* Address: 0x80136794, Size: 204 bytes (51 insns) */
#include "types.h"

extern u8 now_girl;
extern u32 p_birthday;
extern u8 now_month;
extern u8 now_day;
extern u8 day_flag;
extern u8 ret_sub_sub_step;
extern u8 now_sub_sub_step;
extern u8 girl_no;

s32 get_g_zyotai_s(s32 arg0);
void date_name_set(s32 arg0);
void goto_sub_step(s32 arg0);
s32 sub_sub_step_up(void);

s32 date2_girl_birthday_step(void)
{
    s32 v0;
    u32 v1;
    u32 t7, t0;
    u8 a0;

    v0 = get_g_zyotai_s((s32)now_girl);
    v1 = p_birthday;
    t7 = v1 & 0x0F;
    if (t7 == (u32)now_month)
    {
        t0 = (v1 << 23) >> 27;
        if (t0 == (u32)now_day)
        {
            if ((u32)((u32)v0 & 0x7F) < 2U)
            {
                ret_sub_sub_step = (u8)(now_sub_sub_step + 1);
                day_flag = (day_flag & ~0x08) | 0x04;
                a0 = girl_no;
                now_girl = a0;
                date_name_set((s32)a0);
                goto_sub_step(4);
                return 0;
            }
            day_flag = day_flag | 0x08;
        }
    }
    return sub_sub_step_up();
}
