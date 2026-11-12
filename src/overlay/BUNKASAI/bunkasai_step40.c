/* @SymbolStart: 0x8013C0AC */
/* @RodataBase: 0x8015CBF4 */
/* Address: 0x8013C0B4 */
/* Size: 784 bytes (196 insns) */
/* @SymbolSize: 792 */
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
extern void fade_in(void);
extern void fade_out_step(void);
extern void fade_out(void);
extern void zyotai_sprite_set_chk(void);
extern void serifu_run(void);
extern s32 get_g_zyotai_low(s32);
extern void bustup_load_step(s32);
extern void bunkasai_girl2_home_srf_bank_set(void);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init(void);

extern s16 now_serifu;
extern u8  now_sub_step;
extern u8  now_girl;
extern s32 bunka_sd_kind;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 mes_base_y;

extern u8 now_sub_sub_step;
void bunkasai_step40(void) {
    s32 v0;

    switch (now_sub_sub_step) {
    case 0:
        value_cache_find_or_load(16645, 1);
        sub_sub_step_up();
        break;
    case 1:
        fade_out_step();
        break;
    case 2:
        if (get_CD_State2() & 1) {
            bustup_load_step(21038);
        }
        break;
    case 3:
        v0 = dec_bg_init(1);
        indexed_byte_lsb_set(1, v0);
        bunkasai_basyo_init();
        bustup_ptr_init();
        now_girl = 2;
        zyotai_sprite_set_chk();
        cd_read_start2(10, 0x801E8000, 18988);
        break;
    case 4:
        fade_in();
        break;
    case 5:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(2);
            switch (v0) {

                break;
            case 0: case 1: case 2:
                now_serifu = 62;
                break;
            case 3:
                now_serifu = 63;
                break;
            case 4:
                now_serifu = 64;
                break;

                break;
            }
            bunkasai_girl2_home_srf_bank_set();
        }
        break;
    case 6:
        serifu_run();
        break;
    case 7:
        fade_out();
        break;
    case 8:
        bunka_sd_kind = 3;
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_step(128);
        break;
    case 9:
        bustup_load_step(21038);
        break;
    case 10:
        bunkasai_basyo_init();
        mes_base_y = 192;
        bustup_ptr_init();
        now_girl = 2;
        zyotai_sprite_set_chk();
        cd_read_start2(10, 0x801E8000, 18988);
        break;
    case 11:
        fade_in();
        break;
    case 12:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(2);
            switch (v0) {

                break;
            case 0: case 1: case 2:
                now_serifu = 65;
                break;
            case 3:
                now_serifu = 66;
                break;
            case 4:
                now_serifu = 67;
                break;

                break;
            }
            bunkasai_girl2_home_srf_bank_set();
        }
        break;
    case 13:
        serifu_run();
        break;
    case 14:
        fade_down_main();
        break;
    case 15:
        fade_out_step();
        break;
    case 16:
        now_serifu = 1;
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
