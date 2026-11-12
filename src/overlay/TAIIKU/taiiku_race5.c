/* Address: 0x80142F6C (true entry 0x80142F64 -- own cross-prologue head) */
/* @SymbolStart: 0x80142F64 */
/* @SymbolSize: 552 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x28:rs:v0:v1 0x30:rs:v0:v1 0x38:rs:v0:v1 */
#include "types.h"
#include "tokimeki/class_w.h"

extern s32 sub_step_up();
extern s32 sub_sub_step_up();
extern s32 device_cmd_send();
extern s32 game_reset();
extern s32 Vblnk_Timer_Init();
extern s32 Vblnk_Timer();
extern void scroll_poly_disp(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);
extern s32 Fade_Out_Init();
extern s32 bri_fade_out();
extern s32 Fade_In_Main();
extern void taiiku_seat_set(s32 a0, s32 a1);
extern void taiiku_result_sprite_init();
extern void taiiku_seat_disp();
extern s32 taiiku_race_count_inc();
extern s32 taiiku_race_run();
extern s32 taiiku_race_scroll_disp();
extern s32 taiiku_goal_check();
extern void taiiku_race_frame();
extern void taiiku_rank_sprite_init();

extern u8 now_sub_sub_step;
extern u8 taiiku_ninin_girl1;
extern s16 taiiku_ninin_scrl_hi;
extern CLASS_W class_w[];

void taiiku_race5(void)
{
    s32 sel;
    s32 v0;

    sel = now_sub_sub_step;
    switch (sel) {
    case 0:
        if (Fade_In_Main() != 0) {
            Vblnk_Timer_Init();
            sub_sub_step_up();
        }
        taiiku_seat_disp();
        break;
    case 1:
        v0 = Vblnk_Timer();
        if ((u32)v0 >= 91U) {
            device_cmd_send(1282);
            if (taiiku_ninin_girl1 != 0) {
                class_w[100].frame = 57;
            } else {
                class_w[100].frame = 45;
                device_cmd_send(2048);
            }
            class_w[100].disp_flag = 164;
            class_w[100].anim_flag = 64;
            sub_sub_step_up();
        }
        taiiku_seat_disp();
        break;
    case 2:
        taiiku_race_count_inc();
        taiiku_seat_set((s16)(taiiku_ninin_scrl_hi / 2), 4);
        taiiku_race_run();
        taiiku_race_frame();
        taiiku_race_scroll_disp();
        if (taiiku_goal_check() != 0) {
            device_cmd_send(1284);
            Fade_Out_Init();
            sub_sub_step_up();
        }
        break;
    case 3:
        scroll_poly_disp((taiiku_ninin_scrl_hi / 4) % 640, 15, 0, 112, 640);
        scroll_poly_disp((taiiku_ninin_scrl_hi / 2) % 128, 15, 112, 128, 128);
        if (bri_fade_out() != 0) {
            taiiku_rank_sprite_init();
            taiiku_result_sprite_init();
            sub_step_up();
        }
        break;
    default:
        game_reset();
        break;
    }
}
