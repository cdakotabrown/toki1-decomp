/* Address: 0x80042BA4 - 0x80042C74 (208 bytes) */
/* Region: GCC (0x80041000 - 0x80080FFF) */
/* Compiler: IDO 5.2 -O2 -G0 -mips1 -EL -non_shared */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "common.h"

extern void debug_hex_show(s32 value, s32 x, s32 y);
extern void debug_number_show(s32 value, s32 x, s32 y);
extern s32 GetSp(void);

extern s32 vblnk_counter;
extern s32 step_frame_cnt;
extern s32 step_state;
extern s32 step_counter;
extern u8 now_step;
extern u8 now_sub_step;
extern u8 now_sub_sub_step;
extern u8 step_mode;

void step_debug_overlay_draw(void)
{

    debug_hex_show(now_sub_sub_step, 144, 88);

    debug_number_show(step_counter, 128, 88);

    debug_hex_show(now_sub_step, 144, 80);

    debug_number_show(step_state, 128, 80);

    debug_hex_show(now_step, 144, 72);

    debug_number_show(step_frame_cnt, 128, 72);

    debug_hex_show(vblnk_counter, 128, 96);

    debug_hex_show(step_mode, 144, 96);

    debug_hex_show(GetSp(), 144, 104);
}
