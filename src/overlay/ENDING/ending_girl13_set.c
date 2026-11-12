/* @SymbolStart: 0x8013B94C */
/* @SymbolSize: 212 */
/* @PostProc: scheduler_normalizer */
/* @PostProc: patch_insn_reg_field 0x10:rt:t6:a0 0x14:rs:t6:a0 0x14:rt:t6:a0 0x1c:rs:t6:a0 0x48:rt:t7:t6 0x50:rt:t7:t6 0x70:rt:t0:t7 0x74:rs:t0:t7 0x74:rt:t0:t7 0x84:rt:t0:t7 0x88:rt:t3:a0 0x94:rs:t3:a0 0x94:rt:t3:a0 0xa4:rs:t1:a0 0xb4:rt:t2:t0 0xbc:rt:t2:t0 0xc4:rs:t3:t1 0xc4:rt:t4:t2 0xc8:rt:t4:t2 */
/* @PostProc: swap_instructions --offset1 0x94 --offset2 0xB0 */
/* @PostProc: patch_insn_reg_field 0x94:rs:t1:a0 0x94:rt:t1:a0 0xB0:rs:a0:t1 0xB0:rt:a0:t1 */
/* ending_girl13_set - ENDING overlay, Address: 0x8013B94C, Size: 212 bytes (53 instructions) */
#include "types.h"

extern void date_name_set();
extern void ending_ptr_init();
extern void sub_sub_step_up();
extern s32 step_counter;
extern u8  now_girl;
extern s16 now_serifu;
extern s16 now_serifu_in;
extern s32 ss_serifu;
extern s32 ss_serifu_kind;
extern s32 sss_sound;
extern s32 ending_serifu_tbl;
extern s32 ending_serifu_kind_tbl;
extern s32 ending_sound_tbl;
extern u8  ev_flag52;
extern u8  now_sub_sub_step;

s32 ending_girl13_set(void)
{
    u8  mode = ev_flag52;
    u8  t1;
    s32 v1;
    u32 lt;

    if (mode != 0) {
        v1 = step_counter;
        lt = (u32)v1 < 120U;
        v1 += 1;
        step_counter = v1;
        if (lt != 0) {
            return 0;
        }
    }
    now_girl = 13;
    now_serifu = 0;
    now_serifu_in = 0;
    date_name_set();
    ending_ptr_init();
    ss_serifu_kind = ending_serifu_kind_tbl;
    sss_sound = ending_sound_tbl;
    ss_serifu = ending_serifu_tbl;
    if (ev_flag52 != 0) {
        now_serifu = 2;
        t1 = now_sub_sub_step;
        now_sub_sub_step = (u8)(t1 + 10);
    }
    sub_sub_step_up();
}
