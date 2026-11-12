/* @SymbolStart: 0x8014A1AC */
/* @RodataBase: 0x8015E450 */
/* Address: 0x8014A1B4 */
/* Size: 744 bytes (186 insns) */
/* @SymbolSize: 752 */
#include "types.h"

extern void sub_sub_step_up(void);
extern void goto_sub_sub_step(s32);
extern s32 get_CD_State2(void);
extern void cd_read_start2(s32, s32, s32);
extern void game_reset(void);
extern void indexed_byte_lsb_set(s32, s32);
extern void value_cache_find_or_load(s32, s32);
extern s32 dec_bg_init(s32);
extern void bustup_cd_read(s32);
extern void bustup_ptr_init(void);
extern void fade_up_main(void);
extern void fade_down_main(void);
extern void fade_in_step(void);
extern void fade_out_step(void);
extern void zyotai_sprite_set_chk(void);
extern void serifu_run(void);
extern s32 get_g_zyotai_low(s32);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init(void);
extern void bunkasai_param_set(void);

extern s16 now_serifu;
extern u8  now_girl;

extern u8 now_sub_sub_step;
void bunkasai_girl0_club4_96_scene(void) {
    s32 idx = now_sub_sub_step;

    s32 v0;

    switch (idx) {
    case 0:
        value_cache_find_or_load(16664, 1);
        sub_sub_step_up();
        break;
    case 1: case 15:
        fade_out_step();
        break;
    case 2:
        if (get_CD_State2() & 1) {
            bunkasai_basyo_init();
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            bustup_cd_read(21578);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in_step();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            bustup_ptr_init();
            now_girl = 0;
            zyotai_sprite_set_chk();
            now_serifu = 44;
        }
        break;
    case 5: case 9: case 11: case 14: case 17:
        serifu_run();
        break;
    case 6:
        cd_read_start2(17, 0x801E8000, 18879);
        sub_sub_step_up();
        break;
    case 7:
        fade_up_main();
        break;
    case 8:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(0);
            switch (v0) {

                break;
            case 0: case 1:
                now_serifu = 115;
                break;
            case 2:
                now_serifu = 116;
                break;
            case 3:
                now_serifu = 121;
                break;
            case 4:
                now_serifu = 122;
                break;
            }
            bunkasai_param_set();
        }
        break;
    case 10:
        sub_sub_step_up();
        v0 = get_g_zyotai_low(0);
        switch (v0) {

            break;
        case 0: case 1:
            now_serifu = 1;
            break;
        case 2:
            now_serifu = 117;
            break;
        case 3:
            now_serifu = 3;
            break;
        case 4:
            now_serifu = 123;
            goto_sub_sub_step(12);
            break;
        }
        break;
    case 12:
        fade_down_main();
        break;
    case 13:
        v0 = get_g_zyotai_low(0);
        switch (v0) {

            break;
        case 0: case 1:
            goto_sub_sub_step(15);
            break;
        case 2: case 3:
            now_serifu = 120;
            sub_sub_step_up();
            break;
        case 4:
            sub_sub_step_up();
            break;

            break;
        }
        break;
    case 16:
        now_serifu = 0;
        sub_sub_step_up();
        break;





    case 18:
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
