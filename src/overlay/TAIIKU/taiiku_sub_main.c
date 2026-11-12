/* @RodataBase: 0x80148C10 */
#include "types.h"

extern void bustup_ani(void);
extern void sub_sub_step_up(void);
extern void sub_step_up(void);
extern void bustup_load_step(s32);
extern void bustup_ptr_init(void);
extern void zyotai_sprite_set_chk(void);
extern void bustup_face_change(s32);
extern void fade_up_main(void);
extern void date_name_set(s32);
extern void taiiku_ptrs_set(void);
extern void goto_sub_sub_step(s32);
extern void serifu_run(void);
extern void shared_system_state_reset(void);
extern void game_reset(void);
extern void common_disp(void);

extern u8  now_sub_sub_step;
extern u8  pad_type;
extern u8  now_girl;
extern s32 pad_trig;
extern s16 now_serifu;
extern s16 now_serifu_in;
extern s32 ss_serifu;
extern s32 ss_serifu_kind;
extern s32 sss_sound;
extern s32 taiiku_main_serifu;
extern s32 taiiku_main_serifu_1;
extern s32 taiiku_main_serifu_2;

void taiiku_sub_main(void)
{
    bustup_ani();
    switch (now_sub_sub_step) {
    case 0:
        switch (pad_type) {
        case 0:
        case 255:
            sub_sub_step_up();
            break;
        case 1:
            sub_step_up();
            break;
        }
        break;
    case 1:
        bustup_load_step(21533);
        break;
    case 2:
        now_girl = 0;
        bustup_ptr_init();
        zyotai_sprite_set_chk();
        bustup_face_change(1);
        break;
    case 3:
        fade_up_main();
        break;
    case 4:
        date_name_set(0);
        taiiku_ptrs_set();
        now_serifu = 3;
        now_serifu_in = 0;
        ss_serifu = taiiku_main_serifu;
        ss_serifu_kind = taiiku_main_serifu_1;
        sss_sound = taiiku_main_serifu_2;
        if (pad_type == 255)
            goto_sub_sub_step(6);
        else
            sub_sub_step_up();
        break;
    case 5:
        serifu_run();
        break;
    case 6:
        now_serifu = 4;
        sub_sub_step_up();
        break;
    case 7:
        serifu_run();
        break;
    case 8:
        if (pad_type == 1)
            sub_sub_step_up();
        break;
    case 9:
        serifu_run();
        break;
    case 10:
        if (pad_trig != 0) {
            shared_system_state_reset();
            sub_step_up();
        }
        break;
    default:
        game_reset();
        break;
    }
    common_disp();
}
