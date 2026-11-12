/* @CompileFlags: -cckr */
/* @RodataBase: 0x8015FEE4 */
/* @SymbolStart: 0x80158764 */
/* @SymbolSize: 1652 */
#include "types.h"

extern s16 now_serifu;
extern u8 now_sub_sub_step;
extern u8  now_year;
extern s16 girl1_syoushin;
extern s32 girl1_flag;
extern u8  girl1_basyo;
extern u8  now_girl;
extern u8  now_sub_step;
extern s32 bunka_sd_kind;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 mes_base_y;

bunkasai_girl1_club5_scene()
{
    s32 v0;
    u32 b;

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
        break;
    case 4:
        if (now_year == 95) {
            now_serifu = 46;
        } else {
            now_serifu = 47;
        }
        sub_sub_step_up();
        break;
    case 5:
        serifu_run();
        break;
    case 6:
        fade_out_step();
        break;
    case 7:
        bunka_sd_kind = 2;
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_step(128);
        break;
    case 8:
        bustup_load_step(23873);
        break;
    case 9:
        bunkasai_basyo_init2();
        mes_base_y = 192;
        bustup_ptr_init();
        now_girl = 1;
        zyotai_sprite_set_chk();
        cd_read_start2(18, 0x801E8000, 19095);
        break;
    case 10:
        fade_in();
        break;
    case 11:
        if (get_CD_State2() & 1) {
            switch (get_g_zyotai_low(1)) {
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
                now_serifu = 4;
                break;
            }
            bunkasai_girl1_clubs3_5_srf_bank_set();
        }
        break;
    case 12:
        serifu_run();
        break;
    case 13:
        if (get_g_zyotai_low(1) == 4) {
            now_serifu = 9;
        } else {
            now_serifu = 141;
        }
        sub_sub_step_up();
        break;
    case 14:
        serifu_run();
        break;
    case 15:
        sub_sub_step_up();
        switch (get_g_zyotai_low(1)) {
        case 0:
        case 1:
            now_serifu = 142;
            break;
        case 2:
            now_serifu = 143;
            break;
        case 3:
            now_serifu = 144;
            break;
        case 4:
            goto_sub_sub_step(21);
            break;
        }
        if (now_year == 96) {
            now_serifu += 24;
        } else if (now_year == 97) {
            now_serifu += 27;
        }
        break;
    case 16:
        serifu_run();
        break;
    case 17:
        if ((girl1_syoushin & 3) == 0) {
            switch (get_g_zyotai_low(1)) {
            case 0:
            case 1:
                now_serifu = 163;
                break;
            case 2:
                now_serifu = 164;
                break;
            case 3:
                now_serifu = 165;
                break;
            }
        } else {
            switch (get_g_zyotai_low(1)) {
            case 0:
            case 1:
                now_serifu = 145;
                break;
            case 2:
                now_serifu = 146;
                break;
            case 3:
                now_serifu = 147;
                break;
            }
            b = girl1_basyo;
            if (b >> 4 == 1) {
                now_serifu += 3;
            }
            if (now_year == 97 && b >> 4 == 0) {
                now_serifu += 27;
            }
        }
        sub_sub_step_up();
        break;
    case 18:
        serifu_run();
        break;
    case 19:
        switch (get_g_zyotai_low(1)) {
        case 0:
        case 1:
            now_serifu = 151;
            break;
        case 2:
            now_serifu = 152;
            break;
        case 3:
            now_serifu = 153;
            break;
        }
        if (((girl1_flag << 29) >> 31) == 0) {
            now_serifu += 3;
        }
        if (girl1_basyo >> 4 == 1) {
            now_serifu += 6;
        } else if (now_year == 97) {
            now_serifu += 24;
        }
        sub_sub_step_up();
        if ((girl1_syoushin & 3) == 0) {
            goto_sub_sub_step(21);
        }
        break;
    case 20:
        serifu_run();
        break;
    case 21:
        fade_down_main();
        break;
    case 22:
        if (get_g_zyotai_low(1) == 4) {
            now_serifu = 181;
            sub_sub_step_up();
            break;
        }
        if ((girl1_syoushin & 3) == 0) {
            if ((u32)get_g_zyotai_low(1) < 2) {
                now_serifu = 13;
            } else {
                now_serifu = 12;
            }
            sub_sub_step_up();
        } else {
            goto_sub_sub_step(24);
        }
        break;
    case 23:
        serifu_run();
        break;
    case 24:
        fade_out_step();
        break;
    case 25:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 26:
        serifu_run();
        break;
    case 27:
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
