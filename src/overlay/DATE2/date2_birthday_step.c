/* @PostProc: scheduler_normalizer */
/* @PostProc: rename_register_chain --chain t5:t6 --chain t4:t5 --chain t3:t4 --chain t2:t3 --chain t1:t2 */
/* @PostProc: patch_insn_reg_field 0x58:rt:t6:t7 0x94:rs:t6:t7 0x94:rt:t6:t7 0x9c:rs:t6:t7 0x9c:rt:t7:t9 0xa0:rt:t7:t9 */
#include "types.h"

extern u32 p_birthday;
extern u8 now_month;
extern u8 now_day;
extern u8 day_flag;
extern u8 ret_sub_sub_step;
extern u8 now_sub_sub_step;

s32 get_g_zyotai_s(s32 arg0);
void goto_sub_step(s32 arg0);
void sub_sub_step_up(void);

s32 date2_birthday_step(void)
{
    s32 v0;
    u32 v1;
    u32 t7, t0;

    v0 = get_g_zyotai_s(0);
    v1 = p_birthday;
    t7 = v1 & 0x0F;
    if (t7 == (u32)now_month)
    {
        t0 = (v1 << 23) >> 27;
        if (t0 == (u32)now_day)
        {
            if ((u32)((u32)v0 & 0x7F) < 2U)
            {
                day_flag = day_flag | 0x04;
                ret_sub_sub_step = (u8)(now_sub_sub_step + 1);
                goto_sub_step(4);
                return 0;
            }
            day_flag = day_flag | 0x08;
        }
    }
    sub_sub_step_up();
}
