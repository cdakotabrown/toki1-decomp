/* @SymbolStart: 0x8014A778 */
/* @RodataBase: 0x8015E544 */
/* Address: 0x8014A780 */
/* Size: 980 bytes (245 insns) */
/* @SymbolSize: 988 */
/* @CompileFlags: -cckr */
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
extern void bunkasai_param_set(void);

extern s16 now_serifu;
extern u8  now_year;
extern u8  now_girl;
extern u8  now_sub_step;
extern s32 bunka_sd_kind;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 mes_base_y;

extern u8 now_sub_sub_step;
void bunkasai_main1b(void) {
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
            indexed_byte_lsb_set(1, dec_bg_init(1));
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
        bustup_load_step(21578);
        break;
    case 9:
        if (get_CD_State2() & 1) {
            bunkasai_basyo_init2();
            mes_base_y = 192;
            bustup_ptr_init();
            now_girl = 0;
            zyotai_sprite_set_chk();
            cd_read_start2(17, 0x801E8000, 18879);
        }
        break;
    case 10:
        fade_in();
        break;
    case 11:
        if (get_CD_State2() & 1) {
            switch (get_g_zyotai_low(0)) {

                break;
            case 0: case 1:
                now_serifu = 129;
                break;
            case 2:
                now_serifu = 130;
                break;
            case 3:
                now_serifu = 131;
                break;
            case 4:
                now_serifu = 132;
                break;
            }
            if (now_year == 96) {
                now_serifu += 7;
            }
            if (now_year == 97) {
                now_serifu += 11;
            }
            bunkasai_param_set();
        }
        break;
    case 12:
        serifu_run();
        break;
    case 13:
        switch (get_g_zyotai_low(0)) {
        case 0:
            if (now_year == 95) {
                now_serifu = 133;
            } else {
                now_serifu = 4;
            }
            break;
        case 1:
            if (now_year == 95) {
                now_serifu = 134;
            } else {
                now_serifu = 5;
            }
            break;
        case 2:
            now_serifu = 2;
            break;
        case 3:
            now_serifu = 3;
            break;
        case 4:
            now_serifu = 7;
            break;
        }
        sub_sub_step_up();
        break;
    case 14:
        serifu_run();
        break;
    case 15:
        fade_down_main();
        break;
    case 16:
        fade_out_step();
        break;
    case 17:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 18:
        serifu_run();
        break;
    case 19:
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
