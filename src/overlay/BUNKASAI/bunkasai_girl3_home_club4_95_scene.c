/* @SymbolStart: 0x80146100 */
/* @SymbolSize: 936 */
/* @RodataBase: 0x8015DCC0 */
/* Address: 0x80146108 */
/* Size: 928 bytes (232 insns) */
#include "types.h"

extern u8 now_sub_sub_step;
extern void sub_sub_step_up(void);
extern s32 get_CD_State2(void);
extern void cd_read_start2(s32 a0, s32 a1, s32 a2);
extern void game_reset(void);
extern void indexed_byte_lsb_set(s32 a0, s32 a1);
extern void value_cache_find_or_load(s32 a0, s32 a1);
extern s32 dec_bg_init(s32 a0);
extern void bustup_cd_read(s32 a0);
extern void bustup_face_change(s32 a0);
extern void bustup_ptr_init(void);
extern void fade_down_main(void);
extern void fade_in(void);
extern void fade_out_step(void);
extern void fade_out(void);
extern void zyotai_sprite_set_chk(void);
extern void serifu_run(void);
extern s32 get_g_zyotai_low(s32 a0);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init(void);
extern void bunkasai_girl3_home_srf_bank_set(void);

extern s16 now_serifu;
extern u8 now_girl;
extern u16 bukatu_pt4;

void bunkasai_girl3_home_club4_95_scene(void) {
    s32 dispatch;

    s32 ret;

    dispatch = now_sub_sub_step;
    switch (dispatch) {
    case 0:
        value_cache_find_or_load(16655, 1);
        sub_sub_step_up();
        break;
    case 1:
        fade_out_step();
        break;
    case 2:
        ret = get_CD_State2();
        if (ret & 1) {
            ret = dec_bg_init(1);
            indexed_byte_lsb_set(1, ret);
            bustup_cd_read(22748);
            sub_sub_step_up();
        }
        break;
    case 3:
        ret = get_CD_State2();
        if (ret & 1) {
            bunkasai_basyo_init();
            bustup_ptr_init();
            now_girl = 3;
            zyotai_sprite_set_chk();
            cd_read_start2(12, 0x801E8000, 19196);
        }
        break;
    case 4:
        fade_in();
        break;
    case 5:
        ret = get_CD_State2();
        if (ret & 1) {
            ret = get_g_zyotai_low(3);
            switch (ret) {
            case 0: case 1:
                now_serifu = 2;
                break;
            case 2: case 3:
                now_serifu = 5;
                break;
            case 4:
                now_serifu = 9;
                break;

                break;

                break;
            }
            bunkasai_girl3_home_srf_bank_set();
        }
        break;
    case 6:
        serifu_run();
        break;
    case 7:
        if (get_g_zyotai_low(3) == 2 || get_g_zyotai_low(3) == 3) {
            fade_down_main();
        } else {
            sub_sub_step_up();
        }
        break;
    case 8:
        if (get_g_zyotai_low(3) == 2 || get_g_zyotai_low(3) == 3) {
            serifu_run();
        } else {
            sub_sub_step_up();
        }
        break;
    case 9:
        if (get_g_zyotai_low(3) == 2 || get_g_zyotai_low(3) == 3) {
            fade_out_step();
        } else {
            fade_out();
        }
        break;
    case 10:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 11:
        serifu_run();
        break;
    case 12:
        fade_in();
        break;
    case 13:
        ret = get_g_zyotai_low(3);
        switch (ret) {
        case 0: case 1:
            now_serifu = 3;
            break;
        case 2: case 3:
            now_serifu = 7;
            break;
        case 4:
            now_serifu = 10;



            break;
        }
        if ((u32)bukatu_pt4 < 100U) {
            now_serifu = now_serifu + 1;
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
        now_serifu = 1;
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
