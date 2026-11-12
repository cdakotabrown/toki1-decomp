/* @SymbolStart: 0x8014B548 */
/* @RodataBase: 0x8015E6F4 */
/* Address: 0x8014B550 */
/* Size: 1160 bytes (290 insns) */
/* @SymbolSize: 1168 */
#include "types.h"

extern void sub_sub_step_up(void);
extern void goto_step(s32);
extern void goto_sub_sub_step(s32);
extern s32 get_CD_State2(void);
extern void cd_read_start2(s32, s32, s32);
extern void game_reset(void);
extern void set_dec_bri(s32);
extern void dec_bg_reset(void);
extern void indexed_byte_lsb_set(s32, s32);
extern void value_cache_find_or_load(s32, s32);
extern s32 dec_bg_init(s32);
extern void bustup_ptr_init(void);
extern void fade_down_main(void);
extern void fade_in_step(void);
extern void fade_in(void);
extern void fade_out_step(void);
extern void zyotai_sprite_set_chk(void);
extern void serifu_run(void);
extern s32 get_g_zyotai_low(s32);
extern void bustup_load_step(s32);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init2(void);
extern void bunkasai_girl5_srf_bank_set(void);

extern s16 now_serifu;
extern u8  now_year;
extern u8  now_girl;
extern u8  now_sub_step;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 mes_base_y;

extern u8 now_sub_sub_step;
void bunkasai_girl5_club1_scene(void) {
    s32 v0;

    switch (now_sub_sub_step) {
    case 0:
        value_cache_find_or_load(16628, 1);
        sub_sub_step_up();
        break;
    case 1:
        fade_out_step();
        break;
    case 2:
        if (get_CD_State2() & 1) {
            bunkasai_basyo_init2();
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in_step();
        if (now_year == 95) {
            now_serifu = 39;
        } else {
            now_serifu = 40;
        }
        break;
    case 4:
        serifu_run();
        break;
    case 5:
        fade_out_step();
        break;
    case 6:
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_step(128);
        break;
    case 7:
        dec_bg_reset();
        set_dec_bri(0);
        value_cache_find_or_load(16628, 1);
        sub_sub_step_up();
        break;
    case 8:
        if (get_CD_State2() & 1) {
            bustup_load_step(24503);
        }
        break;
    case 9:
        v0 = dec_bg_init(1);
        indexed_byte_lsb_set(1, v0);
        bunkasai_basyo_init2();
        mes_base_y = 192;
        bustup_ptr_init();
        now_girl = 5;
        zyotai_sprite_set_chk();
        cd_read_start2(13, 0x801E8000, 19045);
        break;
    case 10:
        fade_in();
        break;
    case 11:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(5);
            switch (v0) {

                break;
            case 0: case 1:
                now_serifu = 5;
                break;
            case 2:
                now_serifu = 2;
                break;
            case 3:
                now_serifu = 3;
                break;
            case 4:
                now_serifu = 6;
                break;
            }
            bunkasai_girl5_srf_bank_set();
        }
        break;
    case 12:
        serifu_run();
        break;
    case 13:
        v0 = get_g_zyotai_low(5);
        switch (v0) {

            break;
        case 0: case 1:
            now_serifu = now_year * 3 - 258;
            break;
        case 2:
            now_serifu = now_year * 3 - 257;
            break;
        case 3:
            now_serifu = now_year * 3 - 256;
            break;
        case 4:
            now_serifu = 14;
            break;
        }
        sub_sub_step_up();
        break;
    case 14:
        serifu_run();
        break;
    case 15:
        v0 = get_g_zyotai_low(5);
        if (v0 == 4) {
            goto_sub_sub_step(18);
        } else {
            sub_sub_step_up();
        }
        break;
    case 16:
        v0 = get_g_zyotai_low(5);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = 11;
            break;
        case 2:
            now_serifu = 12;
            break;
        case 3:
            now_serifu = 13;
            break;
        }
        sub_sub_step_up();
        break;
    case 17:
        serifu_run();
        break;
    case 18:
        fade_down_main();
        break;
    case 19:
        v0 = get_g_zyotai_low(5);
        if (v0 == 4) {
            goto_sub_sub_step(21);
        } else {
            now_serifu = 111;
            sub_sub_step_up();
        }
        break;
    case 20:
        serifu_run();
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
