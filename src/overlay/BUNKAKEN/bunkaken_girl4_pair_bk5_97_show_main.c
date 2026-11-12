/* @SymbolStart: 0x80139A6C */
/* @RodataBase: 0x80153AE8 */
/* Address: 0x80139A74 */
/* Size: 888 bytes (222 insns) */
/* @SymbolSize: 896 */
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
extern void bunkaken_girl4_pair_bank_init(void);
extern void bunkaken_sub_reset(void);
extern void bunkaken_basyo_init3(void);

extern s16 now_serifu;
extern s16 para_gei;
extern u8  now_sub_step;
extern u8  now_girl;
extern s32 bunka_sd_kind;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern u8 now_sub_sub_step;

void bunkaken_girl4_pair_bk5_97_show_main(void) {
    s32 v0;

    switch (now_sub_sub_step) {
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
            cd_read_start2(12, 0x801E8000, 19282);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(4);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 80;
                break;
            case 2:
            case 3:
                now_serifu = 81;
                break;
            }
            bunkaken_girl4_pair_bank_init();
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
        bustup_load_step(26303);
        break;
    case 9:
        if (get_CD_State2() & 1) {
            bustup_ptr_init();
            now_girl = 4;
            zyotai_sprite_set_chk();
            cd_read_start2(12, 0x801E8000, 19282);
        }
        break;
    case 10:
        fade_in();
        break;
    case 11:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(4);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 86;
                break;
            case 2:
            case 3:
                now_serifu = 90;
                break;
            }
            bunkaken_girl4_pair_bank_init();
        }
        break;
    case 12:
        serifu_run();
        break;
    case 13:
        if (para_gei < 120) {
            now_serifu = now_serifu + 1;
        }
        sub_sub_step_up();
        break;
    case 14:
        serifu_run();
        break;
    case 15:
        v0 = get_g_zyotai_low(4);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = 89;
            break;
        case 2:
        case 3:
            now_serifu = 93;
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
