/* Address: 0x8007C814 */
/* Size: 192 bytes (48 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack_complete --old 32 --new 48 */
/* @PostProc: remap_stack_slots --map 25:43,26:42,27:41 */
#include "types.h"

extern s32 get_CD_State2(void);
extern void sub_sub_step_up(void);
extern void CDQ_init(void);
extern void dec_bg_reset(void);

extern s32 step_counter;
extern u8 bg_change_tbl1;
extern u8 cd_wait_timeout_tbl0;
extern u8 fade_bri;
extern u8 now_sub_sub_step;

void cd_wait_timeout(void) {
    s32 counter;
    u8 save1, save2, save3;
    s32 cond;

    if (get_CD_State2() & 1) {
        sub_sub_step_up();
        return;
    }

    counter = step_counter;
    cond = (u32)counter >= 1025U;
    step_counter = counter + 1;
    if (cond) {
        CDQ_init();

        save1 = bg_change_tbl1;
        save2 = cd_wait_timeout_tbl0;
        save3 = fade_bri;
        dec_bg_reset();
        cd_wait_timeout_tbl0 = save2;
        fade_bri = save3;
        bg_change_tbl1 = save1;
        now_sub_sub_step = now_sub_sub_step - 1;
    }
}
