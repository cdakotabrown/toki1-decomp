/* @SymbolStart: 0x8013DE84 */
/* @SymbolSize: 188 */
/* Address: 0x8013DE84 (cross-prologue head) / 0x8013DE8C (body) */
/* Size: 188 bytes (47 instructions) */
/* @PostProc: scheduler_normalizer */
/* @PostProc: patch_insn_reg_field 0x7c:rt:t3:v0 0x98:rt:t3:v0 */
#include "types.h"

extern void sub_sub_step_up(void);

extern u8  date_pattern;
extern s16 now_serifu;
extern s32 serifu_no;
extern s32 kuchi_stop;
extern u8  now_girl;

void date_basyo8_girl_skip(void) {
    s32 sel;

    sel = (s32)now_girl;
    if (sel == 7 && date_pattern == 0) {
        now_serifu = now_serifu + serifu_no;
        sub_sub_step_up();
    } else if (sel == 10 && date_pattern == 0 && serifu_no == 0) {
        sel = 14;
        kuchi_stop = 1;
        now_girl = (u8)sel;
        sub_sub_step_up();
    } else {
        sub_sub_step_up();
        sub_sub_step_up();
        sub_sub_step_up();
    }
}
