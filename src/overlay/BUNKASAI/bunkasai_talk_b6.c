/* @SymbolStart: 0x80139920 */
/* @RodataBase: 0x8015C734 */
/* Address: 0x80139928 */
/* Size: 1116 bytes (279 insns) */
/* @SymbolSize: 1124 */
#include "types.h"

extern void sub_step_up(void);
extern void sub_sub_step_up(void);
extern void goto_sub_sub_step(s32);
extern void device_cmd_send(s32);
extern s32 get_CD_State2(void);
extern void cd_read_start2(s32, s32, s32);
extern void game_reset(void);
extern void indexed_byte_lsb_set(s32, s32);
extern void value_cache_find_or_load(s32, s32);
extern s32 dec_bg_init(s32);
extern void bustup_ptr_init(void);
extern void fade_up_main(void);
extern void fade_down_main(void);
extern void fade_in_step(void);
extern void fade_out_step(void);
extern void zyotai_sprite_set_chk(void);
extern void serifu_run(void);
extern s32 get_g_zyotai_low(s32);
extern void bustup_load_step(s32);
extern void bunkasai_girl8_srf_bank_set(void);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init(void);

extern s16 now_serifu;
extern u8  now_sub_step;
extern u8  now_sub_sub_step;
extern u8  now_girl;
extern s32 bunka_aisyou;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;

void bunkasai_talk_b6(void) {
    s32 idx = now_sub_sub_step;

    s32 v0;

    switch (idx) {
    case 0:
        value_cache_find_or_load(16645, 1);
        sub_sub_step_up();
        break;
    case 1:
        fade_out_step();
        break;
    case 2:
        if (get_CD_State2() & 1) {
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            sub_sub_step_up();
        }
        break;
    case 3:
        bustup_load_step(20498);
        break;
    case 4:
        bustup_ptr_init();
        now_girl = 8;
        zyotai_sprite_set_chk();
        bunkasai_basyo_init();
        now_serifu = 42;
        break;
    case 5:
        fade_in_step();
        break;
    case 6:
        serifu_run();
        break;
    case 7:
        cd_read_start2(20, 0x801E8000, 18833);
        sub_sub_step_up();
        break;
    case 8:
        fade_up_main();
        break;
    case 9:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(8);
            switch (v0) {

                break;
            case 0: case 1:
                now_serifu = 41;
                break;
            case 2:
                now_serifu = 42;
                break;
            case 3:
                now_serifu = 43;
                break;
            case 4:
                now_serifu = 44;
                break;
            }
            bunkasai_girl8_srf_bank_set();
        }
        break;
    case 10:
        serifu_run();
        break;
    case 11:
        fade_down_main();
        break;
    case 12:
        v0 = get_g_zyotai_low(8);
        if (v0 == 4) {
            serifu_run();
        } else {
            sub_sub_step_up();
        }
        break;
    case 13:
        device_cmd_send(514);
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        sub_step_up();
        break;
    case 14:
        cd_read_start2(20, 0x801E8000, 18833);
        sub_sub_step_up();
        break;
    case 15:
        v0 = get_g_zyotai_low(8);
        if (v0 == 4) {
            sub_sub_step_up();
        } else {
            fade_up_main();
        }
        break;
    case 16:
        if (get_CD_State2() & 1) {
            bunkasai_girl8_srf_bank_set();
            v0 = get_g_zyotai_low(8);
            switch (v0) {

                break;
            case 0: case 1:
                now_serifu = 46;
                break;
            case 2:
                now_serifu = 49;
                break;
            case 3:
                now_serifu = 52;
                break;
            case 4:
                now_serifu = 55;
                break;
            }
            now_serifu = now_serifu + bunka_aisyou;
        }
        break;
    case 17:
        serifu_run();
        break;
    case 18:
        sub_sub_step_up();
        v0 = get_g_zyotai_low(8);
        switch (v0) {

            break;
        case 0: case 1:
            now_serifu = 58;
            break;
        case 2:
            now_serifu = 59;
            break;
        case 3:
            now_serifu = 60;
            break;
        case 4:
            goto_sub_sub_step(21);
            break;
        }
        break;
    case 19:
        serifu_run();
        break;
    case 20:
        v0 = get_g_zyotai_low(8);
        if (v0 == 4) {
            sub_sub_step_up();
        } else {
            fade_down_main();
        }
        break;
    case 21:
        fade_out_step();
        break;
    case 22:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 23:
        serifu_run();
        break;
    case 24:
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
