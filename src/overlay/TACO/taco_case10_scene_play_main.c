/* @SymbolStart: 0x80137F70 */
/* @SymbolSize: 140 */
/* @PostProc: patch_insn_reg_field 0x18:rt:v0:v1 0x1c:rs:v0:v1 0x1c:rt:v0:v1 0x30:rs:v0:v1 0x3c:rs:v0:v1 0x44:rs:v0:v1 */
/* Address: 0x80137F70 (cross-prologue head) / 0x80137F80 (prologue) */
#include "types.h"

extern s32 step_counter;
extern u8 now_sub_sub_step;

extern void taco_play(void);
extern void taco_scene_init(void);
extern void taco_tex_load(void);
extern void taco_main8_main2(void);
extern void taco_time_disp(void);

void taco_case10_scene_play_main(void)
{
    s32 val;
    s32 sel;

    val = step_counter;
    sel = now_sub_sub_step;

    step_counter = val + 1;

    switch (sel) {
    case 0:
        taco_scene_init();
        break;
    case 1:
        taco_tex_load();
        break;
    case 2:
        taco_main8_main2();
        break;
    }
    taco_time_disp();
    taco_play();
}
