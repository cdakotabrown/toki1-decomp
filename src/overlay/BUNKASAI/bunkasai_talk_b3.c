/* @SymbolStart: 0x80138EF8 */
/* @SymbolSize: 704 */
/* @RodataBase: 0x8015C5F0 */
#include "types.h"

extern void sub_sub_step_up(void);
extern void goto_step(s32 a0);
extern s32 get_CD_State2(void);
extern void cd_read_start2(s32 a0, s32 a1, s32 a2);
extern void game_reset(void);
extern void indexed_byte_lsb_set(s32 a0, s32 a1);
extern void value_cache_find_or_load(s32 a0, s32 a1);
extern s32 dec_bg_init(s32 a0);
extern void bustup_ptr_init(void);
extern void fade_down_main(void);
extern void fade_in_step(void);
extern void fade_in(void);
extern void fade_out_step(void);
extern void zyotai_sprite_set_chk(void);
extern void serifu_run(void);
extern s32 get_g_zyotai_low(s32 a0);
extern void bustup_load_step(s32 a0);
extern void bunkasai_girl8_srf_bank_set(void);
extern void bunkasai_basyo_init2(void);
extern void bunkasai_basyo_init(void);
extern void bunkasai_sub_reset(void);

extern s16 now_serifu;
extern u8 now_girl;
extern u8 now_sub_step;
extern s32 bunka_sd_kind;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 mes_base_y;

extern u8 now_sub_sub_step;
void bunkasai_talk_b3(void) {

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
        now_serifu = 38;
        sub_sub_step_up();
        break;
    case 5:
        serifu_run();
        break;
    case 6:
        fade_out_step();
        break;
    case 7:
        bunka_sd_kind = 0;
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_step(128);
        break;
    case 8:
        bustup_load_step(20498);
        break;
    case 9:
        bustup_ptr_init();
        now_girl = 8;
        zyotai_sprite_set_chk();
        bunkasai_basyo_init();
        mes_base_y = 192;
        cd_read_start2(20, 0x801E8000, 18833);
        break;
    case 10:
        fade_in();
        break;
    case 11:
        if (get_CD_State2() & 1) {
            switch (get_g_zyotai_low(8)) {
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
            case 4:
                now_serifu = 14;
                break;
            }
            bunkasai_girl8_srf_bank_set();
        }
        break;
    case 12:
        serifu_run();
        break;
    case 13:
        fade_down_main();
        break;
    case 14:
        if (get_g_zyotai_low(8) == 4) {
            serifu_run();
        } else {
            sub_sub_step_up();
        }
        break;
    case 15:
        fade_out_step();
        break;
    case 16:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 17:
        serifu_run();
        break;
    case 18:
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
