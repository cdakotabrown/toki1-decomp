/* @SymbolStart: 0x8013BD2C */
/* @SymbolSize: 180 */
/* @PostProc: swap_registers a2:v1 --bidirectional */
/* @PostProc: swap_instructions --offset1 0x50 --offset2 0x54 */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x64 */
/* @PostProc: patch_insn_reg_field 0x74:rd:t5:t4 0x7c:rd:t6:t5 0x80:rt:t6:t5 0xa0:rt:t4:t3 0xa8:rs:t5:t4 0xa8:rt:t4:t3 */
#include "types.h"

extern void sub_sub_step_up(void);
extern u8  screen_w[];
extern u8  girl_appear[];
extern s32 shougatu_appear_ss_serifu;
extern s32 shougatu_appear_ss_serifu_kind;
extern s32 shougatu_appear_sss_sound;
extern s16 shougatu_appear_now_serifu;
extern s16 shougatu_appear_now_serifu_in;
extern s32 shougatu_appear_serifu_tbl[];
extern s32 shougatu_appear_serifu_kind_tbl[];
extern s32 shougatu_appear_sound_tbl[];

void shougatu_girl_appear_init(void)
{
    s16 i;
    u8 t;

    shougatu_appear_ss_serifu = shougatu_appear_serifu_tbl[11];
    shougatu_appear_ss_serifu_kind = shougatu_appear_serifu_kind_tbl[11];
    shougatu_appear_sss_sound = shougatu_appear_sound_tbl[11];
    shougatu_appear_now_serifu = 0;
    shougatu_appear_now_serifu_in = 0;
    for (i = 0; i < 11; i++) {
        t = screen_w[i * 56 + 0x1C9] & 1;
        girl_appear[i] = ((u32)t < 1U) ^ 1;
    }
    sub_sub_step_up();
}
