/* @SymbolStart: 0x80153530 */
/* @SymbolSize: 652 */
/* @RodataBase: 0x8015F5E4 */
/* Address: 0x80153538 */
/* Size: 644 bytes (161 insns) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void goto_sub_sub_step(s32 a0);
extern s32 get_CD_State2(void);
extern void cd_read_start2(s32 a0, s32 a1, s32 a2);
extern void game_reset(void);
extern void indexed_byte_lsb_set(s32 a0, s32 a1);
extern void value_cache_find_or_load(s32 a0, s32 a1);
extern s32 dec_bg_init(s32 a0);
extern void bustup_ptr_init(void);
extern void fade_down_main(void);
extern void fade_in(void);
extern void fade_out_step(void);
extern void zyotai_sprite_set_chk(void);
extern void serifu_run(void);
extern s32 get_g_zyotai_low(s32 a0);
extern void bustup_load_step(s32 a0);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init(void);
extern void bunkasai_girl0_home_srf_bank_set(void);

extern s16 now_serifu;
extern u8 now_girl;

extern u8 now_sub_sub_step;
void bunkasai_mainb(void) {
    s32 dispatch = now_sub_sub_step;

    s32 ret;

    switch (dispatch) {
    case 0:
        value_cache_find_or_load(16664, 1);
        sub_sub_step_up();
        break;
    case 1:
        fade_out_step();
        break;
    case 2:
        ret = get_CD_State2();
        if (ret & 1) {
            bustup_load_step(21668);
        }
        break;
    case 3:
        ret = dec_bg_init(1);
        indexed_byte_lsb_set(1, ret);
        bunkasai_basyo_init();
        bustup_ptr_init();
        now_girl = 0;
        zyotai_sprite_set_chk();
        cd_read_start2(24, 0x801E8000, 18896);
        break;
    case 4:
        fade_in();
        break;
    case 5:
        ret = get_CD_State2();
        if (ret & 1) {
            ret = get_g_zyotai_low(0);
            switch (ret) {
            case 0: case 1:
                now_serifu = 138;
                break;
            case 2:
                now_serifu = 140;
                break;
            case 3:
                now_serifu = 141;
                break;
            case 4:
                now_serifu = 142;
                break;

                break;
            }
            bunkasai_girl0_home_srf_bank_set();
        }
        break;
    case 6:
        serifu_run();
        break;
    case 7:
        fade_down_main();
        break;
    case 8:
        if (get_g_zyotai_low(0) == 2 || get_g_zyotai_low(0) == 3) {
            goto_sub_sub_step(10);
        } else {
            sub_sub_step_up();
        }
        break;
    case 9:
        serifu_run();
        break;
    case 10:
        if (get_g_zyotai_low(0) == 2 || get_g_zyotai_low(0) == 3) {
            fade_down_main();
        } else {
            sub_sub_step_up();
        }
        break;
    case 11:
        fade_out_step();
        break;
    case 12:
        now_serifu = 1;
        sub_sub_step_up();
        break;
    case 13:
        serifu_run();
        break;
    case 14:
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
