/* @SymbolStart: 0x801347A8 */
/* @SymbolSize: 744 */
/* @RodataBase: 0x8015BD44 */
/* Address: 0x801347B0 */
/* Size: 736 bytes (184 insns) */
#include "types.h"

extern u8 now_sub_sub_step;
extern void sub_sub_step_up(void);
extern void goto_sub_sub_step(s32 a0);
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
extern void bustup_face_change(s32 a0);
extern void zyotai_sprite_set_chk(void);
extern void serifu_run(void);
extern s32 get_g_zyotai_low(s32 a0);
extern void bunkasai_girl2_vst_bank_init(void);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init(void);

extern s16 now_serifu;
extern u8 now_girl;
extern u16 bukatu_pt4;

void bunkasai_girl2_vst_bk4_95_exh_main(void) {
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
            bunkasai_basyo_init();
            ret = dec_bg_init(1);
            indexed_byte_lsb_set(1, ret);
            bustup_cd_read(21038);
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
            now_girl = 2;
            zyotai_sprite_set_chk();
            now_serifu = 43;
        }
        break;
    case 5:
        serifu_run();
        break;
    case 6:
        cd_read_start2(26, 0x801E8000, 18962);
        sub_sub_step_up();
        break;
    case 7:
        fade_up_main();
        break;
    case 8:
        ret = get_CD_State2();
        if (ret & 1) {
            ret = get_g_zyotai_low(2);
            switch (ret) {
            case 0: case 1:
                now_serifu = 152;
                break;
            case 2:
                now_serifu = 157;
                break;
            case 3:
                now_serifu = 162;
                break;
            case 4:
                now_serifu = 167;
                break;

                break;
            }
            bunkasai_girl2_vst_bank_init();
        }
        break;
    case 9:
        serifu_run();
        break;
    case 10:
        ret = get_g_zyotai_low(2);
        if (ret == 4) {
            fade_down_main();
        } else {
            if (bukatu_pt4 < 100U) {
                bustup_face_change(4);
                now_serifu = now_serifu + 2;
            }
            sub_sub_step_up();
        }
        break;
    case 11:
        serifu_run();
        break;
    case 12:
        ret = get_g_zyotai_low(2);
        if (ret == 4) {
            goto_sub_sub_step(14);
        } else {
            fade_down_main();
        }
        break;
    case 13:
        serifu_run();
        break;
    case 14:
        fade_out_step();
        break;
    case 15:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 16:
        serifu_run();
        break;
    case 17:
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
