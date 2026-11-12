/* @SymbolStart: 0x801464A8 */
/* @SymbolSize: 596 */
/* @RodataBase: 0x8015DD38 */
/* Address: 0x801464B0 */
/* Size: 588 bytes (147 insns) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void goto_sub_sub_step(s32 a0);
extern s32 get_CD_State2(void);
extern void cd_read_start2(s32 a0, s32 a1, s32 a2);
extern void game_reset();
extern void indexed_byte_lsb_set(s32 a0, s32 a1);
extern void value_cache_find_or_load(s32 a0, s32 a1);
extern s32 dec_bg_init(s32 a0);
extern void bustup_cd_read(s32 a0);
extern void bustup_ptr_init(void);
extern void fade_down_main(void);
extern void fade_in(void);
extern void fade_out_step(void);
extern void zyotai_sprite_set_chk(void);
extern void serifu_run(void);
extern s32 get_g_zyotai_low(s32 a0);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init(void);
extern void bunkasai_girl3_home_srf_bank_set(void);

extern s16 now_serifu;
extern u8 now_girl;

extern u8 now_sub_sub_step;
void bunkasai_girl3_home_club4_96_scene(void) {
    s32 t6 = now_sub_sub_step;

    s32 result;
    s32 v0;

    switch (t6) {
    case 0:
        value_cache_find_or_load(16664, 1);
        sub_sub_step_up();
        break;

    case 1:
        fade_out_step();
        break;

    case 2:
        result = get_CD_State2();
        if (result & 1) {
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            bustup_cd_read(22748);
            sub_sub_step_up();
        }
        break;

    case 3:
        result = get_CD_State2();
        if (result & 1) {
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
        result = get_CD_State2();
        if (result & 1) {
            v0 = get_g_zyotai_low(3);
            switch (v0) {
            case 0: case 1:
                now_serifu = 12;
                break;
            case 2:
                now_serifu = 13;
                break;
            case 3:
                now_serifu = 14;
                break;
            case 4:
                now_serifu = 15;
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
        fade_down_main();
        break;

    case 8:
        v0 = get_g_zyotai_low(3);
        if (v0 == 4) {
            now_serifu = 69;
            sub_sub_step_up();
        } else {
            goto_sub_sub_step(10);
        }
        break;

    case 9:
        serifu_run();
        break;

    case 10:
        fade_out_step();
        break;

    case 11:
        now_serifu = 1;
        sub_sub_step_up();
        break;

    case 12:
        serifu_run();
        break;

    case 13:
        bunkasai_sub_reset();
        break;

    default:
        game_reset();
        break;
    }
}
