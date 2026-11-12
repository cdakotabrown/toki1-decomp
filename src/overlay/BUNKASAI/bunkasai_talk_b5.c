/* @SymbolStart: 0x80139504 */
/* @RodataBase: 0x8015C6B0 */
/* Address: 0x8013950C */
/* Size: 1044 bytes (261 insns) */
/* @SymbolSize: 1052 */
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
extern void bunkasai_girl8_srf_bank_set(void);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init(void);

extern s16 now_serifu;
extern u16 basyo_pack;
extern u8  now_year;
extern u8  now_sub_step;
extern u8  now_girl;
extern s32 bunka_sd_kind;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 mes_base_y;

extern u8 now_sub_sub_step;
void bunkasai_talk_b5(void) {
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
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            sub_sub_step_up();
        }
        break;
    case 3:
        bustup_load_step(20498);
        break;
    case 4:
        bustup_ptr_init();
        now_girl = 8;
        zyotai_sprite_set_chk();
        bunkasai_basyo_init();
        now_serifu = 41;
        break;
    case 5:
        fade_in_step();
        break;
    case 6:
        serifu_run();
        break;
    case 7:
        cd_read_start2(20, 0x801E8000, 18833);
        sub_sub_step_up();
        break;
    case 8:
        fade_up_main();
        break;
    case 9:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(8);
            switch (v0) {

                break;
            case 0: case 1:
                now_serifu = 27;
                break;
            case 2:
                now_serifu = 28;
                break;
            case 3:
                now_serifu = 29;
                break;
            case 4:
                now_serifu = 30;
                break;
            }
            bunkasai_girl8_srf_bank_set();
        }
        break;
    case 10:
        serifu_run();
        break;
    case 11:
        fade_out();
        break;
    case 12:
        bunka_sd_kind = 0;
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_step(128);
        break;
    case 13:
        bustup_load_step(20498);
        break;
    case 14:
        bustup_ptr_init();
        now_girl = 8;
        zyotai_sprite_set_chk();
        bunkasai_basyo_init();
        mes_base_y = 192;
        cd_read_start2(20, 0x801E8000, 18833);
        break;
    case 15:
        v0 = get_g_zyotai_low(8);
        if (v0 == 4) {
            fade_in_step();
        } else {
            fade_in();
        }
        break;
    case 16:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(8);
            switch (v0) {

                break;
            case 0: case 1:
                now_serifu = now_year * 3 - 254;
                break;
            case 2:
                now_serifu = now_year * 3 - 253;
                break;
            case 3:
                now_serifu = now_year * 3 - 252;
                break;
            case 4:
                now_serifu = 40;
                break;
            }
            bunkasai_girl8_srf_bank_set();
        }
        break;
    case 17:
        serifu_run();
        break;
    case 18:
        v0 = get_g_zyotai_low(8);
        if (v0 == 4) {
            sub_sub_step_up();
        } else {
            fade_down_main();
        }
        break;
    case 19:
        fade_out_step();
        break;
    case 20:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 21:
        serifu_run();
        break;
    case 22:
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
