/* @SymbolStart: 0x80148B84 */
/* @RodataBase: 0x8015E148 */
/* Address: 0x80148B8C */
/* Size: 1036 bytes (259 insns) */
/* @SymbolSize: 1044 */
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
extern void bunkasai_param_set(void);

extern s16 now_serifu;
extern u8  now_year;
extern u8  now_sub_step;
extern u8  now_girl;
extern s32 bunka_sd_kind;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 mes_base_y;

extern u8 now_sub_sub_step;
void bunkasai_girl0_club2_scene(void) {
    s32 v0;

    switch (now_sub_sub_step) {
    case 0:
        value_cache_find_or_load(16636, 1);
        sub_sub_step_up();
        break;
    case 1: case 20:
        fade_out_step();
        break;
    case 2:
        if (get_CD_State2() & 1) {
            bunkasai_basyo_init();
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            bustup_cd_read(21578);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in_step();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            bustup_ptr_init();
            now_girl = 0;
            zyotai_sprite_set_chk();
            now_serifu = 41;
        }
        break;
    case 5: case 9: case 16: case 18: case 22:
        serifu_run();
        break;
    case 6:
        cd_read_start2(17, 0x801E8000, 18879);
        sub_sub_step_up();
        break;
    case 7:
        fade_up_main();
        break;
    case 8:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(0);
            switch (v0) {

                break;
            case 0: case 1:
                now_serifu = 34;
                break;
            case 2:
                now_serifu = 35;
                break;
            case 3:
                now_serifu = 36;
                break;
            case 4:
                now_serifu = 37;
                break;
            }
            now_serifu = now_serifu + now_year * 8 - 760;
            bunkasai_param_set();
        }
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
        bustup_load_step(21578);
        break;
    case 13:
        bunkasai_basyo_init();
        mes_base_y = 192;
        bustup_ptr_init();
        now_girl = 0;
        zyotai_sprite_set_chk();
        cd_read_start2(17, 0x801E8000, 18879);
        break;
    case 14:
        v0 = get_g_zyotai_low(0);
        if (v0 == 4) {
            fade_in_step();
        } else {
            fade_in();
        }
        break;
    case 15:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(0);
            switch (v0) {

                break;
            case 0: case 1:
                now_serifu = 38;
                break;
            case 2:
                now_serifu = 39;
                break;
            case 3:
                now_serifu = 40;
                break;
            case 4:
                now_serifu = 41;
                break;
            }
            now_serifu = now_serifu + now_year * 8 - 760;
            bunkasai_param_set();
        }
        break;
    case 17:
        sub_sub_step_up();
        v0 = get_g_zyotai_low(0);
        switch (v0) {

            break;
        case 0: case 1:
            now_serifu = 1;
            break;
        case 2:
            now_serifu = 118;
            break;
        case 3:
            now_serifu = 3;
            break;
        case 4:
            goto_sub_sub_step(20);
            break;
        }
        break;
    case 19:
        fade_down_main();
        break;
    case 21:
        now_serifu = 0;
        sub_sub_step_up();
        break;





    case 23:
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
