/* @SymbolStart: 0x8013E5A0 */
/* @SymbolSize: 144 */
/* Address: 0x8013E5A0 (cross-prologue head) / 0x8013E5A8 (body) */
/* Size: 144 bytes (36 insns) */
/* @CompileFlags: -Wo,-no_const_in_reg */
/* @PostProc: patch_stack_complete --old 32 --new 48 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
#include "types.h"

extern void serifu_run(void);
extern void sub_sub_step_up(void);

extern u8  now_girl;
extern s32 serifu_no;
extern u8  date_ride_kind;
extern u8  date_pattern;
extern u8  now_sub_sub_step;
extern s16 now_serifu;

void date_basyo12_girl10_serifu(void) {
    s32 prev;
    if (now_girl == 10 && serifu_no == 1 && date_ride_kind == 2 &&
        date_pattern == 1) {
        prev = now_sub_sub_step;
        serifu_run();
        if (prev != now_sub_sub_step) {
            now_serifu = 55;
        }
    } else {
        sub_sub_step_up();
    }
}
