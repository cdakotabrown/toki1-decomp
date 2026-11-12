/* @SymbolStart: 0x80138430 */
/* @SymbolSize: 208 */
/* @PostProc: patch_insn_reg_field 0x18:rt:v0:v1 0x1c:rs:v0:v1 0x1c:rt:v0:v1 0x30:rs:v0:v1 0x3c:rs:v0:v1 0x44:rs:v0:v1 */
/* Address: 0x80138430 (cross-prologue head) / 0x80138440 (prologue) */
#include "types.h"

extern s32 frame_counter;
extern s32 step_state;
extern s32 step_counter;
extern u8 now_sub_sub_step;
extern u8 taco_mainb_str0;

extern void taco_play(void);
extern void play_end_set(void);
extern void taco_mainb_init(void);
extern void taco_load_poll2(void);
extern void taco_stage_go(void);
extern void taco_time_disp(void);
extern void taco_str_disp2(s32, s32, s32, s32);

void taco_mainb(void)
{
    s32 val;
    s32 sel;
    s32 t8;
    s32 t9;
    s32 t0;

    val = step_counter;
    sel = now_sub_sub_step;

    step_counter = val + 1;

    switch (sel) {
    case 0:
        taco_mainb_init();
        break;
    case 1:
        taco_load_poll2();
        break;
    case 2:
        taco_stage_go();
        break;
    }
    t8 = (s32)step_state;
    if ((u32)t8 >= 96U) goto lbl_801384D8;
    t9 = (s32)frame_counter;
    t0 = t9 & 0x10;
    if (t0 == 0) goto lbl_801384D8;
    taco_str_disp2((s32)&taco_mainb_str0, -64, 0, 2);
lbl_801384D8:
    taco_time_disp();
    taco_play();
    play_end_set();
}
