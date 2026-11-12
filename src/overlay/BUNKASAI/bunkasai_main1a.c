/* @SymbolStart: 0x8014A49C */
/* @SymbolSize: 732 */
/* @RodataBase: 0x8015E4D8 */
/* Address: 0x8014A4A4 */
/* Size: 724 bytes (181 insns) */
#include "types.h"

extern void sub_sub_step_up(void);
extern s32 get_CD_State2(void);
extern void cd_read_start2(s32 a0, s32 a1, s32 a2);
extern void game_reset(void);
extern void indexed_byte_lsb_set(s32 a0, s32 a1);
extern void value_cache_find_or_load(s32 a0, s32 a1);
extern s32 dec_bg_init(s32 a0);
extern void bustup_cd_read(s32 a0);
extern void bustup_ptr_init(void);
extern void fade_up_main(void);
extern void fade_down_main(void);
extern void fade_in_step(void);
extern void fade_out_step(void);
extern void zyotai_sprite_set_chk(void);
extern void serifu_run(void);
extern s32 get_g_zyotai_low(s32 a0);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init(void);
extern void bunkasai_param_set(void);

extern s16 now_serifu;
extern u8 now_girl;

extern u8 now_sub_sub_step;
void bunkasai_main1a(void) {
    s32 dispatch = now_sub_sub_step;

    s32 ret;

    switch (dispatch) {
    case 0:
        value_cache_find_or_load(16655, 1);
        sub_sub_step_up();
        break;
    case 1:
    case 13:
        fade_out_step();
        break;
    case 2:
        ret = get_CD_State2();
        if (ret & 1) {
            bunkasai_basyo_init();
            ret = dec_bg_init(1);
            indexed_byte_lsb_set(1, ret);
            bustup_cd_read(21578);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in_step();
        break;
    case 4:
        ret = get_CD_State2();
        if (ret & 1) {
            bustup_ptr_init();
            now_girl = 0;
            zyotai_sprite_set_chk();
            now_serifu = 45;
        }
        break;
    case 5:
    case 9:
    case 15:
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
        ret = get_CD_State2();
        if (ret & 1) {
            ret = get_g_zyotai_low(0);
            switch (ret) {
            case 0: case 1:
                now_serifu = 124;
                break;
            case 2:
                now_serifu = 125;
                break;
            case 3:
                now_serifu = 126;
                break;
            case 4:
                now_serifu = 127;
                break;

                break;
            }
            bunkasai_param_set();
        }
        break;
    case 10:
        ret = get_g_zyotai_low(0);
        switch (ret) {
        case 0: case 1:
            now_serifu = 1;
            break;
        case 2:
            now_serifu = 2;
            break;
        case 3:
            now_serifu = 3;
            break;
        case 4:
            now_serifu = 128;
            break;

            break;
        }
        sub_sub_step_up();
        break;
    case 11:
        ret = get_g_zyotai_low(0);
        if (ret == 4) {
            fade_down_main();
        } else {
            serifu_run();
        }
        break;
    case 12:
        ret = get_g_zyotai_low(0);
        if (ret == 4) {
            serifu_run();
        } else {
            fade_down_main();
        }
        break;
    case 14:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 16:
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
