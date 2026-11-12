/* @SymbolStart: 0x80148524 */
/* @RodataBase: 0x8015E054 */
/* Address: 0x8014852C */
/* Size: 724 bytes (181 insns) */
/* @SymbolSize: 732 */
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
extern u8  now_sub_step;
extern u8  now_girl;
extern s32 bunka_sd_kind;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 mes_base_y;

extern u8 now_sub_sub_step;
void bunkasai_girl0_club0_97_scene(void) {
    s32 v0;

    switch (now_sub_sub_step) {
    case 0:
        value_cache_find_or_load(16628, 1);
        sub_sub_step_up();
        break;
    case 1: case 6:
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
        now_serifu = 38;
        sub_sub_step_up();
        break;
    case 5: case 12: case 14: case 18:
        serifu_run();
        break;
    case 7:
        bunka_sd_kind = 0;
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_step(128);
        break;
    case 8:
        bustup_load_step(21578);
        break;
    case 9:
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
                now_serifu = 18;
                break;
            case 2:
                now_serifu = 19;
                break;
            case 3:
                now_serifu = 20;
                break;
            case 4:
                now_serifu = 21;
                break;
            }
            bunkasai_param_set();
        }
        break;
    case 13:
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
            break;
        }
        sub_sub_step_up();
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




    case 19:
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
