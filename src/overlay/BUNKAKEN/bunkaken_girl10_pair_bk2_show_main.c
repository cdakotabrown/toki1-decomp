/* @SymbolStart: 0x8013D82C */
/* @RodataBase: 0x801540EC */
/* Address: 0x8013D834 */
/* Size: 948 bytes (237 insns) */
/* @SymbolSize: 956 */
#include "types.h"

extern void sub_sub_step_up(void);
extern void goto_step(s32);
extern s32 get_CD_State2(void);
extern void cd_read_start2(s32, s32, s32);
extern void game_reset(void);
extern void indexed_byte_lsb_set(s32, s32);
extern void value_cache_find_or_load(s32, s32);
extern s32 dec_bg_init(s32);
extern void bustup_ptr_init(void);
extern void fade_in(void);
extern void fade_out(void);
extern void zyotai_sprite_set_chk(void);
extern void serifu_run(void);
extern s32 get_g_zyotai_low(s32);
extern void bustup_load_step(s32);
extern void bunkaken_girl10_pair_bank_init(void);
extern void bunkaken_sub_reset(void);
extern void bunkaken_basyo_init3(void);

extern s16 now_serifu;
extern u8  now_year;
extern u8  now_sub_step;
extern u8  now_girl;
extern s32 bunka_sd_kind;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s32 bunka_bukatu;
extern s16 mes_base_y;
extern u8 now_sub_sub_step;

void bunkaken_girl10_pair_bk2_show_main(void) {
    s32 v0;

    switch (now_sub_sub_step) {
    case 0:
        if (bunka_bukatu == 2) {
            value_cache_find_or_load(16636, 1);
        } else {
            value_cache_find_or_load(16645, 1);
        }
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
            cd_read_start2(13, 0x801E8000, 19314);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(10);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 20;
                break;
            case 2:
                now_serifu = 21;
                break;
            case 3:
                now_serifu = 22;
                break;
            }
            if (bunka_bukatu == 3) {
                now_serifu = now_serifu + 3;
            }
            bunkaken_girl10_pair_bank_init();
        }
        break;
    case 5:
        serifu_run();
        break;
    case 6:
        fade_out();
        break;
    case 7:
        bunka_sd_kind = 1;
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_step(128);
        break;
    case 8:
        bustup_load_step(26933);
        break;
    case 9:
        bunkaken_basyo_init3();
        mes_base_y = 192;
        bustup_ptr_init();
        now_girl = 10;
        zyotai_sprite_set_chk();
        cd_read_start2(13, 0x801E8000, 19314);
        break;
    case 10:
        fade_in();
        break;
    case 11:
        if (get_CD_State2() & 1) {
            if (bunka_bukatu == 2) {
                v0 = get_g_zyotai_low(10);
                switch (v0) {
                case 0:
                case 1:
                    now_serifu = 26;
                    break;
                case 2:
                    now_serifu = 27;
                    break;
                case 3:
                    now_serifu = 28;
                    break;
                }
                if (now_year == 97) {
                    now_serifu = now_serifu + 3;
                }
            } else {
                v0 = get_g_zyotai_low(10);
                switch (v0) {
                case 0:
                case 1:
                    now_serifu = 32;
                    break;
                case 2:
                    now_serifu = 33;
                    break;
                case 3:
                    now_serifu = 34;
                    break;
                }
            }
            bunkaken_girl10_pair_bank_init();
        }
        break;
    case 12:
        serifu_run();
        break;
    case 13:
        fade_out();
        break;
    case 14:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 15:
        serifu_run();
        break;
    case 16:
        bunkaken_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
