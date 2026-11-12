/* @RodataBase: 0x80154AEC */
/* @SymbolSize: 492 */
/* Address: 0x80144A90 */
/* Size: 492 bytes (123 insns) */
#include "types.h"

extern s32 sub_sub_step_up(void);
extern s32 goto_sub_step(s32);
extern s32 get_CD_State2(void);
extern s32 cd_read_start2(s32, s32, s32);
extern s32 game_reset(void);
extern s32 bustup_ptr_init(void);
extern s32 fade_up_main(void);
extern s32 fade_down_main(void);
extern s32 date_name_set(s32);
extern s32 serifu_run(void);
extern s32 bustup_load_step(s32);
extern s32 zyotai_sprite_set_chk(void);
extern void bunkaken_common_bank_set(void);

extern s16 now_serifu;
extern u16 now_serifu_in;
extern s32 ss_serifu;
extern s32 ss_serifu_kind;
extern s32 sss_sound;
extern u8  now_year;
extern u8  now_girl;
extern s32 bunkaken_bank_ptr[];
extern u8 now_sub_sub_step;

void bunkaken_girl13_greet_main(void) {
    s32 idx;
    s32 v0;

    idx = now_sub_sub_step;

    switch (idx) {
    case 0:
        bustup_load_step(27743);
        break;
    case 1:
        bustup_ptr_init();
        now_girl = 13;
        zyotai_sprite_set_chk();
        cd_read_start2(5, 0x801E8000, 18874);
        break;
    case 2:
        fade_up_main();
        break;
    case 3:
        if (get_CD_State2() & 1) {
            date_name_set(13);
            bunkaken_common_bank_set();
            now_serifu = 6;
            now_serifu_in = 0;
            ss_serifu = bunkaken_bank_ptr[0];
            ss_serifu_kind = bunkaken_bank_ptr[1];
            sss_sound = bunkaken_bank_ptr[2];
            sub_sub_step_up();
        }
        break;
    case 4:
        serifu_run();
        break;
    case 5:
        v0 = now_year;
        switch (v0) {
        case 95:
            break;
        case 96:
            now_serifu = now_serifu + 1;
            break;
        case 97:
            now_serifu = now_serifu + 2;
            break;
        default:
            game_reset();
            break;
        }
        sub_sub_step_up();
        break;
    case 6:
        serifu_run();
        break;
    case 7:
        fade_down_main();
        break;
    case 8:
        now_serifu = 10;
        sub_sub_step_up();
        break;
    case 9:
        serifu_run();
        break;
    case 10:
        goto_sub_step(3);
        break;
    default:
        game_reset();
        break;
    }
}
