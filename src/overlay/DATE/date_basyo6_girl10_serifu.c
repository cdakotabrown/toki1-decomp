/* @SymbolStart: 0x8013E350 */
/* @SymbolSize: 292 */
/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=1 */
/* @PostProc: swap_instructions --offset1 0xd8 --offset2 0xdc */
/* @PostProc: swap_instructions --offset1 0xd4 --offset2 0xd8 */
/* @PostProc: swap_instructions --offset1 0xd0 --offset2 0xd4 */
/* @PostProc: swap_instructions --offset1 0xcc --offset2 0xd0 */
/* @PostProc: patch_insn_reg_field 0x20:rs:v1:v0 0x20:rt:v0:v1 0x84:rs:v1:v0 0x84:rt:v0:v1 */
/* Function: date_basyo6_girl10_serifu  (true symbol starts 0x8013E350) */
/* Overlay: DATE */
#include "types.h"

extern s32 serifu_run();
extern s32 sub_sub_step_up();

extern u8  now_girl;
extern s32 serifu_no;
extern u8  date_pattern;
extern u8  step_mode;
extern s16 now_serifu;
extern s16 now_serifu_in;
extern s16 para_ri;
extern s32 kuchi_stop;
extern u8  now_sub_sub_step;
extern u8  girl_no;

s32 date_basyo6_girl10_serifu(void)
{
    u32 step;
    volatile s32 saved;

    step = now_girl;

    if (step == 10 && serifu_no == 0 && date_pattern == 0) {
        if (step_mode == 0) {
            now_serifu = 7;
            now_serifu_in = 0;
            step_mode = 1;
        }
        return serifu_run();
    }

    if (step == 10 && serifu_no == 2 && date_pattern == 1 &&
        para_ri < 120) {
        if (now_serifu_in == 1) {
            kuchi_stop = 1;
            now_girl = 14;
        }
        saved = now_sub_sub_step;
        serifu_run();
        if (saved != now_sub_sub_step) {
            kuchi_stop = 0;
        }
        now_girl = girl_no;
    } else {
        return sub_sub_step_up();
    }

}
