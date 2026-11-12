/* @SymbolStart: 0x801398A4 */
/* @SymbolSize: 272 */
/* @PostProc: patch_insn_reg_field 0x4c:rt:v1:v0 0x50:rs:v1:v0 0x50:rt:v1:v0 0x5c:rs:v1:v0 0x6c:rs:v1:v0 0xc0:rt:t4:v0 0xc4:rs:t4:v0 0xc4:rt:t4:v0 0xcc:rs:t4:v0 0xcc:rt:t5:t4 0xd4:rt:t5:t4 0xe8:rt:t8:t7 0xec:rt:t6:t5 0xf0:rs:t6:t5 0xf0:rt:t6:t5 0xf8:rs:t6:t5 0xf8:rt:t7:t6 0xfc:rt:t7:t6 0x104:rt:t8:t7 */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 serifu_run();

extern s32 serifu_no;
extern u8  now_girl;
extern volatile u8 now_sub_sub_step;
extern s16 now_serifu;
extern u8  date_basyo12_cmd_flag;

s32 date_basyo12_serifu_skip12(void)
{
    u32 v;
    volatile s16 saved;
    u32 cur;

    if (serifu_no == 1) {
        v = now_girl;
        if (v == 7) {
            saved = now_sub_sub_step;
            serifu_run();
            cur = now_sub_sub_step;
            if (cur != saved) {
                now_serifu = 10;
                now_sub_sub_step = cur + 12;
            }
            return 0;
        }
        now_serifu = 9;
        if (v == 9) {
            now_serifu = 8;
        } else if (v == 4 || v == 8 || v == 5) {
            now_serifu = 7;
        }
        now_sub_sub_step = now_sub_sub_step + 12;
    } else {
        if (now_girl == 7) {
            now_serifu = now_serifu + 1;
        }
        date_basyo12_cmd_flag = 1;
    }
    return sub_sub_step_up();
}
