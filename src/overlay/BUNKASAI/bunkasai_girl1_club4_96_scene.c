/* @SymbolStart: 0x80157DD4 */
/* @RodataBase: 0x8015FDE8 */
/* Address: 0x80157DDC */
/* Size: 784 bytes (196 insns) */
/* @SymbolSize: 792 */
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
extern void bunkasai_girl1_clubs3_5_srf_bank_set(void);

extern s16 now_serifu;
extern u8  now_girl;

extern u8 now_sub_sub_step;
void bunkasai_girl1_club4_96_scene(void) {
    s32 idx = now_sub_sub_step;

    s32 v0;

    switch (idx) {
    case 0:
        value_cache_find_or_load(16664, 1);
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
            bustup_cd_read(23873);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in_step();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            bustup_ptr_init();
            now_girl = 1;
            zyotai_sprite_set_chk();
            now_serifu = 44;
        }
        break;
    case 5:
        serifu_run();
        break;
    case 6:
        cd_read_start2(18, 0x801E8000, 19095);
        sub_sub_step_up();
        break;
    case 7:
        fade_up_main();
        break;
    case 8:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(1);
            switch (v0) {

                break;
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
                now_serifu = 5;
                break;
            }
            bunkasai_girl1_clubs3_5_srf_bank_set();
        }
        break;
    case 9:
        serifu_run();
        break;
    case 10:
        now_serifu = 106;
        sub_sub_step_up();
        break;
    case 11:
        serifu_run();
        break;
    case 12:
        v0 = get_g_zyotai_low(1);
        switch (v0) {

            break;
        case 0: case 1:
            now_serifu = 107;
            break;
        case 2:
            now_serifu = 108;
            break;
        case 3:
            now_serifu = 109;
            break;
        case 4:
            now_serifu = 110;
            break;
        }
        sub_sub_step_up();
        break;
    case 13:
        serifu_run();
        break;
    case 14:
        fade_down_main();
        break;
    case 15:
        now_serifu = 111;
        sub_sub_step_up();
        break;
    case 16:
        serifu_run();
        break;
    case 17:
        fade_out_step();
        break;
    case 18:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 19:
        serifu_run();
        break;
    case 20:
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
