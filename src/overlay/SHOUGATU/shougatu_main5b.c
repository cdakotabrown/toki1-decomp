/* @RodataBase: 0x801437D0 */
/* Address: 0x80137124 */
/* Size: 568 bytes (142 instructions) */
/* @PostProc: patch_insn_reg_field 0xa4:rt:v0:v1 0xac:rs:v0:v1 0xac:rt:t8:v0 0xb0:rs:t8:v0 0xb0:rt:t9:v0 0xb4:rs:v0:v1 0xb8:rs:t9:v0 0xd0:rt:t0:t8 0xd4:rs:t0:t8 0xd4:rt:t0:t8 0xdc:rs:t0:t8 0xe4:rd:t0:t8 0xe4:rt:t0:t8 0xec:rt:t0:t8 0xf0:rt:t0:t8 0xf8:rs:t0:t8 */
/* @SymbolSize: 568 */
#include "types.h"

extern void bustup_ani(void);
extern void shougatu_scene_init(void);
extern void shougatu_load_wait(void);
extern void shougatu_cdload16(void);
extern s32 get_CD_State2(void);
extern void sub_step_up(void);
extern void goto_sub_step(s32);
extern void game_reset(void);
extern void shougatu_part1(void);
extern void shougatu_part2(void);
extern void shougatu_part3(void);
extern void shougatu_part4(void);
extern void shougatu_part5(void);
extern void shougatu_part6(void);
extern void shougatu_subsub_run(void);
extern void shougatu_part8(void);
extern void shougatu_dispatch_run(void);
extern void shougatu_bgm_wait(void);
extern void shougatu_schedule_return2(void);
extern void k_scroll_set(void);
extern void k_disp_wait(void);
extern void update_anim_state(s32);
extern void message_window_show(void);
extern void basyo_show(void);
extern void hizuke_show(void);
extern void dir_scroll_main(void);
extern void don_move(void);

extern u8 now_sub_step;
extern s32 step_state;
extern u8 shougatu_date_girl;

void shougatu_main5b(void)
{
    s32 n;

    bustup_ani();
    switch (now_sub_step) {
    case 0:
        shougatu_scene_init();
        break;
    case 1:
        shougatu_load_wait();
        break;
    case 2:
        shougatu_cdload16();
        break;
    case 3:
        if (get_CD_State2() & 1) {
            sub_step_up();
            break;
        }
        n = step_state;
        step_state = n + 1;
        if (((u32)n < 1025) ^ 1) {
            goto_sub_step(2);
        }
        break;
    case 4:
        switch (shougatu_date_girl) {
        case 0:
            game_reset();
            break;
        case 1:
            shougatu_part1();
            break;
        case 2:
            shougatu_part2();
            break;
        case 3:
            shougatu_part3();
            break;
        case 4:
            shougatu_part4();
            break;
        case 5:
            shougatu_part5();
            break;
        case 6:
            shougatu_part6();
            break;
        case 7:
            shougatu_subsub_run();
            break;
        case 8:
            shougatu_part8();
            break;
        case 9:
            game_reset();
            break;
        case 10:
            shougatu_dispatch_run();
            break;
        default:
            game_reset();
            break;
        }
        break;
    case 5:
        shougatu_bgm_wait();
        break;
    case 6:
        shougatu_load_wait();
        break;
    case 7:
        shougatu_schedule_return2();
        break;
    default:
        game_reset();
        break;
    }

    k_scroll_set();
    k_disp_wait();
    update_anim_state(2);
    message_window_show();
    basyo_show();
    hizuke_show();
    dir_scroll_main();
    don_move();
}
