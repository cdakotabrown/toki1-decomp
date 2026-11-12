/* Function: step_up */
/* Address:  0x80042948 (the .sym recorded 0x8004298C) */
/* Size:     112 bytes (28 instructions) */
/* Region:   Game (0x80041000-0x80085FFF) - IDO 5.2 */
/* @SymbolStart: 0x80042948 */
/* @SymbolSize: 112 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
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
extern void step_state_save_and_redirect(void);

void step_up(void) {
    now_step = now_step + 1;
    now_sub_step = 0;
    now_sub_sub_step = 0;
    step_frame_cnt = 0;
    step_state = 0;
    step_counter = 0;
    scene_entry_b = 0;
    scene_entry_c = 0;
    step_mode = 0;
    step_state_save_and_redirect();
}
