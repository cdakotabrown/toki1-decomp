/* Address: 0x80042A28 (the .sym recorded the prologue, 0x80042A60) */
/* Size: 144 bytes (36 instructions) */
/* @SymbolStart: 0x80042A28 */
/* @SymbolSize: 144 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: swap_instructions --offset1 0x54 --offset2 0x58 */
#include "types.h"

extern u8 now_step;
extern u8 now_sub_step;
extern u8 now_sub_sub_step;
extern s32 step_frame_cnt;
extern s32 step_state;
extern s32 step_counter;
extern u8 scene_entry_b;
extern u8 scene_entry_c;
extern u8 step_mode;
extern u8 prev_step;
extern s16 sound_seek_time;
extern void step_state_save_and_redirect(void);
extern void mes_window_switch(s32);

void goto_step(s32 step) {
    now_step = step;
    now_sub_step = 0;
    now_sub_sub_step = 0;
    step_frame_cnt = 0;
    step_state = 0;
    step_counter = 0;
    scene_entry_b = 0;
    scene_entry_c = 0;
    step_mode = 0;
    step_state_save_and_redirect();
    sound_seek_time = 0;
    if (prev_step != 94 && prev_step != 80) {
        mes_window_switch(0);
    }
}
