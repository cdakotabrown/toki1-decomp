/* @SymbolStart: 0x80159AD4 */
/* @RodataBase: 0x801600F0 */
/* Address: 0x80159ADC */
/* Size: 1248 bytes (312 insns) */
/* @SymbolSize: 1256 */
#include "types.h"

extern void sub_sub_step_up(void);
extern void goto_step(s32);
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
extern void fade_in(void);
extern void fade_out_step(void);
extern void fade_out(void);
extern void zyotai_sprite_set_chk(void);
extern void serifu_run(void);
extern s32 get_g_zyotai_low(s32);
extern void bustup_load_step(s32);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init(void);
extern void bunkasai_girl9_srf_bank_set(void);

extern s16 now_serifu;
extern u8  now_year;
extern u16 basyo_pack;
extern u8  now_girl;
extern u8  now_sub_step;
extern s32 bunka_sd_kind;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 mes_base_y;

extern u8 now_sub_sub_step;
void bunkasai_step95(void) {
    s32 v0;

    switch (now_sub_sub_step) {
    case 0:
        if (((u32)basyo_pack >> 12) == 2) {
            value_cache_find_or_load(16636, 1);
        } else {
            value_cache_find_or_load(16645, 1);
        }
        sub_sub_step_up();
        break;
    case 1:
        fade_out_step();
        break;
    case 2:
        if (get_CD_State2() & 1) {
            bunkasai_basyo_init();
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            bustup_cd_read(25808);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in_step();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            bustup_ptr_init();
            now_girl = 9;
            zyotai_sprite_set_chk();
            now_serifu = 41;
        }
        break;
    case 5:
        serifu_run();
        break;
    case 6:
        cd_read_start2(10, 0x801E8000, 19251);
        sub_sub_step_up();
        break;
    case 7:
        fade_up_main();
        break;
    case 8:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(9);
            switch (v0) {

                break;
            case 0: case 1:
                now_serifu = 24;
                break;
            case 2:
                now_serifu = 25;
                break;
            case 3:
                now_serifu = 26;
                break;
            case 4:
                now_serifu = 27;
                break;
            }
            bunkasai_girl9_srf_bank_set();
        }
        break;
    case 9:
        serifu_run();
        break;
    case 10:
        fade_out();
        break;
    case 11:
        bunka_sd_kind = 0;
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_step(128);
        break;
    case 12:
        bustup_load_step(25808);
        break;
    case 13:
        bunkasai_basyo_init();
        mes_base_y = 192;
        bustup_ptr_init();
        now_girl = 9;
        zyotai_sprite_set_chk();
        cd_read_start2(10, 0x801E8000, 19251);
        break;
    case 14:
        v0 = get_g_zyotai_low(9);
        if (v0 == 4) {
            fade_in_step();
            break;
        }

        fade_in();
        break;
    case 15:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(9);
            switch (v0) {

                break;
            case 0: case 1:
                if (now_year == 96) {
                    now_serifu = 28;
                } else {
                    now_serifu = 32;
                }
                break;
            case 2:
                if (now_year == 96) {
                    now_serifu = 29;
                } else {
                    now_serifu = 33;
                }
                break;
            case 3:
                if (now_year == 96) {
                    now_serifu = 30;
                } else {
                    now_serifu = 34;
                }
                break;
            case 4:
                now_serifu = 31;
                break;
            }
            bunkasai_girl9_srf_bank_set();
        }
        break;
    case 16:
        serifu_run();
        break;
    case 17:
        sub_sub_step_up();
        v0 = get_g_zyotai_low(9);
        switch (v0) {

            break;
        case 0: case 1:
            now_serifu = 1;
            break;
        case 2:
            now_serifu = 3;
            break;
        case 3:
            now_serifu = 4;
            break;
        case 4:
            goto_sub_sub_step(21);
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
        if ((u32)get_g_zyotai_low(9) < 2) {
            serifu_run();
        } else {
            sub_sub_step_up();
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
