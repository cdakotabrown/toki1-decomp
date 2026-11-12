/* @SymbolStart: 0x80138660 */
/* @SymbolSize: 188 */
/* /* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x28:rs:v0:v1 0x30:rs:v0:v1 */
/* Address: 0x80138668 */
/* Size: 180 bytes (45 insns) */
#include "types.h"

extern u8 now_sub_sub_step;

extern u8 taco_main15_str0[];
extern s32 frame_counter;
extern s32 step_state;

extern void taco_play();
extern void play_end_set();
extern void func_8013871C();
extern void taco_load_poll();
extern void taco_stage_go2();
extern void taco_time_disp();
extern void taco_str_disp2();

void taco_case31_stage_play_main(void)
{
    u8 v1 = now_sub_sub_step;

    switch (v1) {
    case 0:
        func_8013871C();
        break;
    case 1:
        taco_load_poll();
        break;
    case 2:
        taco_stage_go2();
        break;
    }

    if ((u32)step_state < 96U && (frame_counter & 0x10)) {
        taco_str_disp2(taco_main15_str0, -64, 0, 2);
    }

    taco_time_disp();
    taco_play();
    play_end_set();
}
