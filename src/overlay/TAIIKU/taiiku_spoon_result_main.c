/* @PostProc: patch_stack_complete --old 48 --new 72 */
/* @PostProc: remap_stack_slots --from 40 --to 60 */
/* @RodataBase: 0x80149084 */
/* Address: 0x80145DEC */
/* Size: 716 bytes (179 insns) */
#include "types.h"

extern void sub_step_up(void);
extern void sub_sub_step_up(void);
extern void device_cmd_send(s32 id);
extern void game_reset(void);
extern void Vblnk_Timer_Init(void);
extern u32 Vblnk_Timer(void);
extern void scroll_poly_disp(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);
extern void Fade_Out_Init(void);
extern s32 bri_fade_out(void);
extern void fade_timer_init(void);
extern s32 Fade_In_Main(void);
extern void taiiku_xa_cell_set(s32 a0, s32 a1);
extern s32 taiiku_result_slide(void);
extern s32 taiiku_lane_drop(void);
extern void taiiku_result_frame_sub(void);
extern void taiiku_spoon_result_frame_step(void);

extern s16 back_clear_b[];
extern s16 girl0_tokimeki;
extern s16 girl0_yuukou;
extern u8 now_sub_sub_step;
extern u8 taiiku_spoon_rank[];

void taiiku_spoon_result_main(void)
{
    s32 i;

    scroll_poly_disp(0, 15, 0, 240, 640);
    switch ((u32)now_sub_sub_step) {
    case 0:
        device_cmd_send(1291);
        fade_timer_init();
        sub_sub_step_up();
        break;
    case 1:
        if (Fade_In_Main() != 0) {
            if (taiiku_spoon_rank[0] == 1 || taiiku_spoon_rank[1] == 1) {
                device_cmd_send(517);
            } else {
                device_cmd_send(518);
            }
            sub_sub_step_up();
        }
        break;
    case 2:
        if (taiiku_result_slide() != 0) {
            sub_sub_step_up();
        }
        break;
    case 3:
        if (taiiku_lane_drop() != 0) {
            sub_sub_step_up();
        }
        break;
    case 4:
        taiiku_result_frame_sub();
        for (i = 0; i < 4; i++) {
            if (taiiku_spoon_rank[i] == 1) {
                taiiku_xa_cell_set(11, (u8)i);
            }
        }
        sub_sub_step_up();
        break;
    case 5:
        taiiku_spoon_result_frame_step();
        break;
    case 6:
        Vblnk_Timer_Init();
        sub_sub_step_up();
        break;
    case 7:
        if (Vblnk_Timer() >= 241) {
            Fade_Out_Init();
            sub_sub_step_up();
        }
        break;
    case 8:
        if (bri_fade_out() != 0) {
            if (taiiku_spoon_rank[0] == 1) {
                girl0_tokimeki = girl0_tokimeki + 5;
                for (i = 0; i < 12; i++) {
                    back_clear_b[223 + i * 28] += 5;
                }
            } else if (taiiku_spoon_rank[1] == 1) {
                girl0_yuukou = girl0_yuukou + 3;
                for (i = 0; i < 12; i++) {
                    back_clear_b[225 + i * 28] += 3;
                }
            }
            sub_step_up();
        }
        break;
    default:
        game_reset();
        break;
    }
}
