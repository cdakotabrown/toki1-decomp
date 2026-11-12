/* Address: 0x80134718 */
/* Size: 336 bytes (84 insns) */
/* @PostProc: swap_registers a0:v1 --exclude-offsets 0x80,0x9c,0xe8,0xec */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x28:rs:v0:v1 0x30:rs:v0:v1 0x88:rt:v1:a0 0xa4:rd:v1:a0 0xf0:rt:v1:a0 0xf4:rs:v1:a0 0xf4:rt:v1:a0 */
#include "types.h"

extern s16 taiiku_kari_scrl_hi;
extern s32 load_done_flag;

extern s32 sub_step_up(void);
extern s32 sub_sub_step_up(void);
extern s32 device_cmd_send(s32);
extern s32 device_load_start(s32, s32, s32, s32, s32, s32);
extern s32 game_reset(void);
extern s32 Vblnk_Timer_Init(void);
extern s32 Vblnk_Timer(void);
extern s32 Fade_In_Main(void);
extern s32 load_done_flag_set(void);
extern void taiiku_seat_set(s32, s32);
extern void taiiku_seat_disp(void);
extern void taiiku_seat_plate_set(void);
extern s32 taiiku_runner_count_inc(void);
extern void taiiku_run(void);
extern s32 taiiku_scroll_disp(void);
extern void taiiku_racers_step(void);
extern s32 func_8013753C(void);
extern void taiiku_racer_pos_set(void);
extern void taiiku_goal_check2(void);
extern u8 now_sub_sub_step;

void taiiku_start2(void) {
    s32 v1 = now_sub_sub_step;

    s32 state = v1;
    switch (state) {
    case 0:
        if (Fade_In_Main() != 0) {
            Vblnk_Timer_Init();
            sub_sub_step_up();
        }
        taiiku_seat_disp();
        break;
    case 1:
        if ((u32)Vblnk_Timer() < 91U) {
            goto skip1;
        }
        device_cmd_send(1282);
        load_done_flag = 0;
        device_load_start(0, 0xBFA6, 0xBF87, 0xC115, 0xC0D0, 0xD6C5);
        sub_sub_step_up();
    skip1:
        taiiku_seat_disp();
        break;
    case 2:
        load_done_flag_set();
        taiiku_runner_count_inc();
        taiiku_run();
        taiiku_scroll_disp();
        taiiku_seat_set((s32)taiiku_kari_scrl_hi, 3);
        taiiku_goal_check2();
        taiiku_seat_plate_set();
        taiiku_racers_step();
        taiiku_racer_pos_set();
        func_8013753C();
        if (taiiku_kari_scrl_hi >= 320) {
            sub_step_up();
        }
        break;
    default:
        game_reset();
        break;
    }
}
