/* @SymbolStart: 0x801523A0 */
/* @RodataBase: 0x8015F3E4 */
/* Address: 0x801523A8 */
/* Size: 1024 bytes (256 insns) */
/* @SymbolSize: 1032 */
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
extern void fade_in(void);
extern void fade_out_step(void);
extern void zyotai_sprite_set_chk(void);
extern void serifu_run(void);
extern s32 get_g_zyotai_low(s32);
extern void bustup_load_step(s32);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init(void);
extern void bunkasai_girl0_home_srf_bank_set(void);

extern s16 now_serifu;
extern u8  now_sub_step;
extern u8  now_sub_sub_step;
extern u8  now_girl;
extern s32 bunka_aisyou;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;

void bunkasai_girl0_home_club3_95_scene(void) {
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
            bustup_load_step(21578);
        }
        break;
    case 3:
        if (get_CD_State2() & 1) {
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            bunkasai_basyo_init();
            bustup_ptr_init();
            now_girl = 0;
            zyotai_sprite_set_chk();
            cd_read_start2(24, 0x801E8000, 18896);
        }
        break;
    case 4:
        fade_in();
        break;
    case 5:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(0);
            switch (v0) {

                break;
            case 0: case 1:
                now_serifu = 80;
                break;
            case 2:
                now_serifu = 86;
                break;
            case 3:
                now_serifu = 92;
                break;
            case 4:
                now_serifu = 98;
                break;
            }
            bunkasai_girl0_home_srf_bank_set();
        }
        break;
    case 6:
        serifu_run();
        break;
    case 7:
        v0 = get_g_zyotai_low(0);
        if (v0 == 4) {
            fade_down_main();
        } else {
            serifu_run();
        }
        break;
    case 8:
        v0 = get_g_zyotai_low(0);
        if (v0 == 4) {
            serifu_run();
        } else {
            fade_down_main();
        }
        break;
    case 9:
        device_cmd_send(514);
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        sub_step_up();
        break;
    case 10:
        cd_read_start2(24, 0x801E8000, 18896);
        sub_sub_step_up();
        break;
    case 11:
        v0 = get_g_zyotai_low(0);
        if (v0 == 4) {
            sub_sub_step_up();
        } else {
            fade_up_main();
        }
        break;
    case 12:
        if (get_CD_State2() & 1) {
            bunkasai_girl0_home_srf_bank_set();
            v0 = get_g_zyotai_low(0);
            switch (v0) {

                break;
            case 0: case 1:
                now_serifu = 82;
                break;
            case 2:
                now_serifu = 88;
                break;
            case 3:
                now_serifu = 94;
                break;
            case 4:
                now_serifu = 100;
                goto_sub_sub_step(14);
                break;
            }
        }
        break;
    case 13:
        now_serifu = now_serifu + 1;
        sub_sub_step_up();
        break;
    case 14:
        now_serifu = now_serifu + bunka_aisyou;
        sub_sub_step_up();
        break;
    case 15:
        serifu_run();
        break;
    case 16:
        v0 = get_g_zyotai_low(0);
        if (v0 == 4) {
            sub_sub_step_up();
        } else {
            fade_down_main();
        }
        break;
    case 17:
        fade_out_step();
        break;
    case 18:
        now_serifu = 1;
        sub_sub_step_up();
        break;
    case 19:
        serifu_run();
        break;
    case 20:
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
