/* @SymbolStart: 0x80148800 */
/* @RodataBase: 0x8015E0CC */
/* Address: 0x80148808 */
/* Size: 892 bytes (223 insns) */
/* @SymbolSize: 900 */
#include "types.h"

extern void sub_sub_step_up(void);
extern void goto_step(s32);
extern void goto_sub_sub_step(s32);
extern s32 get_CD_State2(void);
extern void cd_read_start2(s32, s32, s32);
extern void game_reset(void);
extern void dec_bg_reset(void);
extern void set_dec_bri(s32);
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
extern u8  now_sub_step;
extern u8  now_girl;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 mes_base_y;

extern u8 now_sub_sub_step;
void bunkasai_girl0_club1_scene(void) {
    s32 v0;

    switch (now_sub_sub_step) {
    case 0:
        value_cache_find_or_load(16628, 1);
        sub_sub_step_up();
        break;
    case 1: case 5: case 17:
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
    case 4: case 12: case 14: case 19:
        serifu_run();
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
            bustup_load_step(21578);
        }
        break;
    case 9:
        v0 = dec_bg_init(1);
        indexed_byte_lsb_set(1, v0);
        bunkasai_basyo_init2();
        mes_base_y = 192;
        bustup_ptr_init();
        now_girl = 0;
        zyotai_sprite_set_chk();
        cd_read_start2(17, 0x801E8000, 18879);
        break;
    case 10:
        fade_in();
        break;
    case 11:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(0);
            switch (v0) {

                break;
            case 0: case 1:
                now_serifu = 22;
                break;
            case 2:
                now_serifu = 23;
                break;
            case 3:
                now_serifu = 24;
                break;
            case 4:
                now_serifu = 25;
                break;
            }
            now_serifu = now_serifu + now_year * 4 - 380;
            bunkasai_param_set();
        }
        break;
    case 13:
        sub_sub_step_up();
        v0 = get_g_zyotai_low(0);
        switch (v0) {
        case 0:
            now_serifu = 4;
            break;
        case 1:
            now_serifu = 5;
            break;
        case 2:
            now_serifu = 2;
            break;
        case 3:
            now_serifu = 3;
            break;
        case 4:
            now_serifu = 7;
            goto_sub_sub_step(15);
            break;
        }
        break;
    case 15:
        fade_down_main();
        break;
    case 16:
        v0 = get_g_zyotai_low(0);
        if (v0 == 4) {
            serifu_run();
        } else {
            sub_sub_step_up();
        }
        break;
    case 18:
        now_serifu = 0;
        sub_sub_step_up();
        break;





    case 20:
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
