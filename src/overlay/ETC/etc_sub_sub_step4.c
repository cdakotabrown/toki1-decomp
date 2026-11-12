/* @SymbolStart: 0x8013C6F0 */
/* @RodataBase: 0x8014CA64 */
/* Address: 0x8013C6F8 */
/* Size: 484 bytes (121 instructions) */
/* @SymbolSize: 492 */
/* @PostProc: swap_instructions --offset1 0xa4 --offset2 0xac */
/* @PostProc: swap_instructions --offset1 0x164 --offset2 0x16c */
/* @PostProc: patch_insn_reg_field 0x78:rt:v1:v0 0x7c:rs:v1:v0 0x7c:rt:v1:v0 0x84:rs:v1:v0 0xb4:rs:v1:v0 0x108:rt:v1:v0 0x10c:rs:v1:v0 0x10c:rt:v1:v0 0x114:rs:v1:v0 0x118:rs:v1:v0 */
#include "types.h"

extern void etc_quad_disp(void);
extern void quad_fade_in_show(void);
extern void etc_quad_mode_disp(s32, s32);
extern void etc_event_load(s32);
extern void etc_record_xa_select(void);
extern void etc_frame_save2(void);
extern void etc_model_matrix_set(void);
extern void sub_sub_step_up(void);

extern s32 etc_event_step;
extern u8  step_mode;
extern u8  now_girl;
extern s32 etc_xa_phase;
extern s32 pad_trig;
extern s32 etc_call_choice;
extern s32 step_counter;
extern s32 etc_model_roty;

s32 etc_sub_sub_step4(void)
{
    s32 v0;
    s32 vb;
    s32 c;

    switch (step_mode) {
    case 0:
        etc_quad_disp();
        quad_fade_in_show();
        break;
    case 1:
        etc_quad_disp();
        etc_quad_mode_disp(0, 0);
        etc_event_load(5);
        v0 = etc_event_step;
        if (v0 >= 3) {
            step_mode++;
            etc_event_step = 0;
            return 0;
        } else if (v0 == -1) {
            step_mode++;
        }
        break;
    case 2:
        etc_quad_disp();
        etc_quad_mode_disp(0, 0);
        v0 = now_girl;
        if (v0 == 12 || v0 == 11) {
            etc_record_xa_select();
        }
        v0 = etc_xa_phase;
        if (v0 == 0) {
            goto c804;
        }
        if (v0 < 3) {
            break;
        }
    c804:
        if (pad_trig & 0x20) {
            goto c830;
        }
        vb = etc_call_choice;
        if (vb == 0) {
            goto c830;
        }
        if (vb != 1) {
            break;
        }
    c830:
        step_mode++;
        etc_xa_phase = 0;
        return 0;
    case 3:
        etc_quad_disp();
        etc_quad_mode_disp(step_counter * 2, 3);
        if ((u32)step_counter < 40) {
            step_counter += 1;
        } else {
            step_mode++;
        }
        break;
    case 4:
        etc_frame_save2();
        etc_model_roty = 2048;
        etc_model_matrix_set();
        sub_sub_step_up();
        break;
    }
}
