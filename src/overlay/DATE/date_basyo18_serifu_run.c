/* @SymbolSize: 108 */
/* @PostProc: patch_stack_complete --old 32 --new 48 */
/* @PostProc: remap_stack_slots --from 31 --to 43 */
/* @PostProc: scheduler_normalizer */
/* @PostProc: patch_insn_reg_field 0x34:rt:v1:a0 0x3c:rs:v1:a0 0x3c:rt:t6:v1 0x40:rs:v1:a0 0x44:rs:t6:v1 0x4c:rt:t7:t6 0x50:rs:t7:t6 0x50:rt:t7:t6 0x58:rs:t7:t6 */
/* Function: date_basyo18_serifu_run */
/* Overlay: DATE */
#include "types.h"

extern s32 get_g_zyotai_h(u8);
extern s32 serifu_run(void);
extern s32 bustup_face_change(u8);

extern u8 now_girl;
extern s32 step_counter;
extern s16 now_serifu_in;

void date_basyo18_serifu_run(void)
{
    u8 saved;
    s32 counter;

    saved = (u8)get_g_zyotai_h(now_girl);
    serifu_run();

    counter = step_counter;
    step_counter = counter + 1;
    if ((counter ^ 1) != 0) {
        return;
    }
    if (now_serifu_in != 2) {
        return;
    }
    bustup_face_change(saved);
}
