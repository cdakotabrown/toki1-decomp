/* @SymbolStart: 0x80148A68 */
/* @RodataBase: 0x80155004 */
/* @SymbolSize: 1088 */
/* Address: 0x80148A70 */
/* Size: 1080 bytes (270 insns) */
#include "types.h"

extern s32 sub_sub_step_up(void);
extern s32 goto_step(s32);
extern s32 goto_sub_sub_step(s32);
extern s32 get_CD_State2(void);
extern s32 cd_read_start2(s32, s32, s32);
extern s32 game_reset(void);
extern s32 indexed_byte_lsb_set(s32, s32);
extern s32 value_cache_find_or_load(s32, s32);
extern s32 dec_bg_init(s32);
extern s32 bustup_ptr_init(void);
extern s32 fade_in(void);
extern s32 fade_out(void);
extern s32 zyotai_sprite_set_chk(void);
extern s32 serifu_run(void);
extern s32 get_g_zyotai_low(s32);
extern s32 bustup_load_step(s32);
extern s32 bunkaken_basyo_init3(void);
extern s32 bunkaken_sub_reset(void);
extern void bunkaken_param_set(void);

extern s16 now_serifu;
extern u8  now_year;
extern s16 para_gei;
extern u8  now_sub_step;
extern u8  now_girl;
extern s32 bunka_sd_kind;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 mes_base_y;
extern u8 now_sub_sub_step;

void bunkaken_girl6_pair_bk5_show_main(void) {
    s32 v0;

    switch ((u32)now_sub_sub_step) {
    case 0:
        value_cache_find_or_load(18044, 1);
        sub_sub_step_up();
        break;
    case 1:
        fade_out();
        break;
    case 2:
        if (get_CD_State2() & 1) {
            bunkaken_basyo_init3();
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            cd_read_start2(16, 0x801E8000, 19235);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(6);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 113;
                break;
            case 2:
                now_serifu = 114;
                break;
            case 3:
                now_serifu = 115;
                break;
            }
            bunkaken_param_set();
        }
        break;
    case 5:
        serifu_run();
        break;
    case 6:
        fade_out();
        break;
    case 7:
        bunka_sd_kind = 0;
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_step(128);
        break;
    case 8:
        bustup_load_step(25133);
        break;
    case 9:
        bunkaken_basyo_init3();
        mes_base_y = 192;
        bustup_ptr_init();
        now_girl = 6;
        zyotai_sprite_set_chk();
        cd_read_start2(16, 0x801E8000, 19235);
        break;
    case 10:
        fade_in();
        break;
    case 11:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(6);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 116;
                break;
            case 2:
                now_serifu = 117;
                break;
            case 3:
                now_serifu = 118;
                break;
            }
            now_serifu = now_serifu + now_year * 3 - 285;
            bunkaken_param_set();
        }
        break;
    case 12:
        serifu_run();
        break;
    case 13:
        if (now_year == 97) {
            v0 = get_g_zyotai_low(6);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 125;
                break;
            case 2:
                now_serifu = 127;
                break;
            case 3:
                now_serifu = 129;
                break;
            }
            if (para_gei < 120) {
                now_serifu = now_serifu + 1;
            }
            sub_sub_step_up();
        } else {
            goto_sub_sub_step(17);
        }
        break;
    case 14:
        serifu_run();
        break;
    case 15:
        v0 = get_g_zyotai_low(6);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = 131;
            break;
        case 2:
            now_serifu = 132;
            break;
        case 3:
            now_serifu = 133;
            break;
        }
        sub_sub_step_up();
        break;
    case 16:
        serifu_run();
        break;
    case 17:
        fade_out();
        break;
    case 18:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 19:
        serifu_run();
        break;
    case 20:
        bunkaken_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
