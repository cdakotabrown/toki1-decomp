/* @SymbolStart: 0x80150234 */
/* @RodataBase: 0x8015F060 */
/* Address: 0x8015023C */
/* Size: 752 bytes (188 insns) */
/* @SymbolSize: 760 */
#include "types.h"

extern void sub_sub_step_up(void);
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
extern void fade_out_step(void);
extern void zyotai_sprite_set_chk(void);
extern void serifu_run(void);
extern s32 get_g_zyotai_low(s32);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init(void);
extern void bunkasai_girl6_srf_bank_set(void);

extern s16 now_serifu;
extern u8  now_girl;

extern u8 now_sub_sub_step;
void bunkasai_girl6_club4_97_scene(void) {
    s32 idx = now_sub_sub_step;

    s32 v0;

    switch (idx) {
    case 0:
        value_cache_find_or_load(16655, 1);
        sub_sub_step_up();
        break;
    case 1:
        fade_out_step();
        break;
    case 2:
        if (get_CD_State2() & 1) {
            bunkasai_basyo_init();
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            bustup_cd_read(25133);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in_step();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            bustup_ptr_init();
            now_girl = 6;
            zyotai_sprite_set_chk();
            now_serifu = 45;
        }
        break;
    case 5:
        serifu_run();
        break;
    case 6:
        cd_read_start2(15, 0x801E8000, 19219);
        sub_sub_step_up();
        break;
    case 7:
        fade_up_main();
        break;
    case 8:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(6);
            switch (v0) {

                break;
            case 0: case 1:
                now_serifu = 104;
                break;
            case 2:
                now_serifu = 105;
                break;
            case 3:
                now_serifu = 106;
                break;
            case 4:
                now_serifu = 107;
                break;
            }
            bunkasai_girl6_srf_bank_set();
        }
        break;
    case 9:
        serifu_run();
        break;
    case 10:
        v0 = get_g_zyotai_low(6);
        switch (v0) {

            break;
        case 0: case 1:
            now_serifu = 1;
            sub_sub_step_up();
            break;
        case 2:
            now_serifu = 2;
            sub_sub_step_up();
            break;
        case 3:
            now_serifu = 3;
            sub_sub_step_up();
            break;
        case 4:
            fade_down_main();
            break;
        }
        break;
    case 11:
        serifu_run();
        break;
    case 12:
        v0 = get_g_zyotai_low(6);
        if (v0 == 4) {
            sub_sub_step_up();
        } else {
            fade_down_main();
        }
        break;
    case 13:
        fade_out_step();
        break;
    case 14:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 15:
        serifu_run();
        break;
    case 16:
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
