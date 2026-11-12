/* @SymbolStart: 0x80140958 */
/* @RodataBase: 0x8015D398 */
/* @SymbolSize: 1524 */
/* Address: 0x80140960 */
/* Size: 1516 bytes (379 insns) */
#include "types.h"

extern u8 now_sub_sub_step;
extern u16 basyo_pack;
extern u8 now_year;
extern u8 now_girl;
extern u8 now_sub_step;
extern s16 now_serifu;
extern s32 bunka_sd_kind;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 mes_base_y;

extern void sub_sub_step_up(void);
extern void goto_step(s32);
extern void goto_sub_sub_step(s32);
extern s32 get_CD_State2(void);
extern void cd_read_start2(s32, u32, s32);
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
extern void bunkasai_girl10_srf_bank_set(void);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init(void);

void bunkasai_step9e(void)
{
    switch ((u32)now_sub_sub_step) {
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
            indexed_byte_lsb_set(1, dec_bg_init(1));
            bustup_cd_read(26933);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in_step();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            bustup_ptr_init();
            now_girl = 10;
            zyotai_sprite_set_chk();
            now_serifu = 41;
        }
        break;
    case 5:
        serifu_run();
        break;
    case 6:
        cd_read_start2(19, 0x801E8000, 19294);
        sub_sub_step_up();
        break;
    case 7:
        fade_up_main();
        break;
    case 8:
        if (get_CD_State2() & 1) {
            switch ((u32)get_g_zyotai_low(10)) {
            case 0:
            case 1:
                now_serifu = 28;
                break;
            case 2:
                now_serifu = 29;
                break;
            case 3:
                now_serifu = 30;
                break;
            case 4:
                now_serifu = 34;
                break;
            }
            if ((u32)get_g_zyotai_low(10) < 4) {
                if (now_year == 97) {
                    now_serifu = now_serifu + 3;
                }
            }
            bunkasai_girl10_srf_bank_set();
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
        bustup_load_step(26933);
        break;
    case 13:
        bunkasai_basyo_init();
        mes_base_y = 192;
        bustup_ptr_init();
        now_girl = 10;
        zyotai_sprite_set_chk();
        cd_read_start2(19, 0x801E8000, 19294);
        break;
    case 14:
        if (get_g_zyotai_low(10) == 4) {
            fade_in_step();
        } else {
            fade_in();
        }
        break;
    case 15:
        if (get_CD_State2() & 1) {
            switch ((u32)get_g_zyotai_low(10)) {
            case 0:
            case 1:
                if (((u32)basyo_pack >> 12) == 2) {
                    now_serifu = 35;
                } else {
                    now_serifu = 64;
                }
                break;
            case 2:
                if (((u32)basyo_pack >> 12) == 2) {
                    now_serifu = 36;
                } else {
                    now_serifu = 65;
                }
                break;
            case 3:
                if (((u32)basyo_pack >> 12) == 2) {
                    now_serifu = 37;
                } else {
                    now_serifu = 66;
                }
                break;
            case 4:
                now_serifu = 41;
                break;
            }
            if ((u32)get_g_zyotai_low(10) < 4) {
                if (now_year == 97) {
                    if (((u32)basyo_pack >> 12) == 2) {
                        now_serifu = now_serifu + 3;
                    }
                }
            }
            bunkasai_girl10_srf_bank_set();
        }
        break;
    case 16:
        serifu_run();
        break;
    case 17:
        sub_sub_step_up();
        switch ((u32)get_g_zyotai_low(10)) {
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
            goto_sub_sub_step(21);
            break;
        }
        if ((u32)get_g_zyotai_low(10) < 4) {
            if (now_year == 97) {
                now_serifu = now_serifu + 3;
            }
        }
        break;
    case 18:
        serifu_run();
        break;
    case 19:
        if (now_year == 97) {
            if ((u32)get_g_zyotai_low(10) < 2) {
                now_serifu = 117;
            }
        }
        fade_down_main();
        break;
    case 20:
        if (get_g_zyotai_low(10) == 4 ||
            (now_year == 97 && (u32)get_g_zyotai_low(10) < 2)) {
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
