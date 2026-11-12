/* @SymbolStart: 0x8014EF24 */
/* @SymbolSize: 1096 */
/* @RodataBase: 0x8015EE20 */
#include "types.h"

extern void sub_step_up(void);
extern void sub_sub_step_up(void);
extern void goto_sub_sub_step(s32 a0);
extern void device_cmd_send(s32 a0);
extern s32 get_CD_State2(void);
extern void cd_read_start2(s32 a0, s32 a1, s32 a2);
extern void game_reset(void);
extern void indexed_byte_lsb_set(s32 a0, s32 a1);
extern void value_cache_find_or_load(s32 a0, s32 a1);
extern s32 dec_bg_init(s32 a0);
extern void bustup_cd_read(s32 a0);
extern void bustup_ptr_init(void);
extern void fade_up_main(void);
extern void fade_down_main(void);
extern void fade_in_step(void);
extern void fade_out_step(void);
extern void zyotai_sprite_set_chk(void);
extern void serifu_run(void);
extern s32 get_g_zyotai_low(s32 a0);
extern void bunkasai_basyo_init(void);
extern void bunkasai_girl6_srf_bank_set(void);
extern void bunkasai_sub_reset(void);

extern s16 now_serifu;
extern u8 now_girl;
extern u8 now_sub_step;
extern u8 now_sub_sub_step;
extern s32 bunka_aisyou;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;

void bunkasai_girl6_club3_95_scene(void) {
    s32 sel = now_sub_sub_step;

    switch (sel) {
    case 0:
        value_cache_find_or_load(16645, 1);
        sub_sub_step_up();
        break;
    case 1:
        fade_out_step();
        break;
    case 2:
        if (get_CD_State2() & 1) {
            bunkasai_basyo_init();
            indexed_byte_lsb_set(1, dec_bg_init(1));
            bustup_cd_read(25133);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in_step();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            now_serifu = 42;
            bustup_ptr_init();
            now_girl = 6;
            zyotai_sprite_set_chk();
        }
        break;
    case 5:
        serifu_run();
        break;
    case 6:
        cd_read_start2(15, 0x801E8000, 19219);
        sub_sub_step_up();
        break;
    case 7:
        fade_up_main();
        break;
    case 8:
        if (get_CD_State2() & 1) {
            switch (get_g_zyotai_low(6)) {
            case 0:
            case 1:
                now_serifu = 47;
                break;
            case 2:
                now_serifu = 48;
                break;
            case 3:
                now_serifu = 49;
                break;
            case 4:
                now_serifu = 50;
                break;
            }
            bunkasai_girl6_srf_bank_set();
        }
        break;
    case 9:
        serifu_run();
        break;
    case 10:
        fade_down_main();
        break;
    case 11:
        if (get_g_zyotai_low(6) == 4) {
            serifu_run();
        } else {
            sub_sub_step_up();
        }
        break;
    case 12:
        device_cmd_send(514);
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        sub_step_up();
        break;
    case 13:
        cd_read_start2(15, 0x801E8000, 19219);
        sub_sub_step_up();
        break;
    case 14:
        if (get_g_zyotai_low(6) == 4) {
            sub_sub_step_up();
        } else {
            fade_up_main();
        }
        break;
    case 15:
        if (get_CD_State2() & 1) {
            bunkasai_girl6_srf_bank_set();
            switch (get_g_zyotai_low(6)) {
            case 0:
            case 1:
                now_serifu = 52;
                break;
            case 2:
                now_serifu = 55;
                break;
            case 3:
                now_serifu = 58;
                break;
            case 4:
                now_serifu = 61;
                break;
            }
            now_serifu = now_serifu + bunka_aisyou;
        }
        break;
    case 16:
        serifu_run();
        break;
    case 17:
        sub_sub_step_up();
        switch (get_g_zyotai_low(6)) {
        case 0:
        case 1:
            now_serifu = 1;
            break;
        case 2:
            now_serifu = 2;
            break;
        case 3:
            now_serifu = 3;
            break;
        case 4:
            goto_sub_sub_step(20);
            break;
        }
        break;
    case 18:
        serifu_run();
        break;
    case 19:
        fade_down_main();
        break;
    case 20:
        fade_out_step();
        break;
    case 21:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 22:
        serifu_run();
        break;
    case 23:
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
