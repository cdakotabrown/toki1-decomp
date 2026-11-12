/* @SymbolStart: 0x8014BFE0 */
/* @RodataBase: 0x8015E834 */
/* Address: 0x8014BFE8 */
/* Size: 1344 bytes (336 insns) */
/* @SymbolSize: 1352 */
#include "types.h"

extern void sub_step_up(void);
extern void sub_sub_step_up(void);
extern void goto_sub_sub_step(s32);
extern void device_cmd_send(s32);
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
extern void bunkasai_girl5_srf_bank_set(void);

extern s16 now_serifu;
extern u8  now_sub_step;
extern u8  now_sub_sub_step;
extern u8  now_girl;
extern s32 bunka_aisyou;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;

void bunkasai_girl5_club3_95_scene(void) {
    s32 idx = now_sub_sub_step;

    s32 v0;

    switch (idx) {
    case 0:
        value_cache_find_or_load(16645, 1);
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
            bustup_cd_read(24503);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in_step();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            now_serifu = 42;
            bustup_ptr_init();
            now_girl = 5;
            zyotai_sprite_set_chk();
        }
        break;
    case 5:
        serifu_run();
        break;
    case 6:
        cd_read_start2(13, 0x801E8000, 19045);
        sub_sub_step_up();
        break;
    case 7:
        fade_up_main();
        break;
    case 8:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(5);
            switch (v0) {

                break;
            case 0: case 1:
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
            bunkasai_girl5_srf_bank_set();
        }
        break;
    case 9:
        serifu_run();
        break;
    case 10:
        v0 = get_g_zyotai_low(5);
        switch (v0) {

            break;
        case 0: case 1:
            now_serifu = 55;
            break;
        case 2:
            now_serifu = 56;
            break;
        case 3:
            now_serifu = 57;
            break;
        case 4:
            now_serifu = 58;
            break;
        }
        sub_sub_step_up();
        break;
    case 11:
        serifu_run();
        break;
    case 12:
        fade_down_main();
        break;
    case 13:
        v0 = get_g_zyotai_low(5);
        if (v0 == 4) {
            serifu_run();
        } else {
            sub_sub_step_up();
        }
        break;
    case 14:
        device_cmd_send(514);
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        sub_step_up();
        break;
    case 15:
        cd_read_start2(13, 0x801E8000, 19045);
        sub_sub_step_up();
        break;
    case 16:
        v0 = get_g_zyotai_low(5);
        if (v0 == 4) {
            sub_sub_step_up();
        } else {
            fade_up_main();
        }
        break;
    case 17:
        if (get_CD_State2() & 1) {
            bunkasai_girl5_srf_bank_set();
        }
        break;
    case 18:
        sub_sub_step_up();
        break;
    case 19:
        now_serifu = bunka_aisyou + 61;
        v0 = get_g_zyotai_low(5);
        if (v0 == 4) {
            now_serifu = now_serifu + 3;
        }
        sub_sub_step_up();
        break;
    case 20:
        serifu_run();
        break;
    case 21:
        sub_sub_step_up();
        v0 = get_g_zyotai_low(5);
        switch (v0) {

            break;
        case 0: case 1:
            now_serifu = 67;
            break;
        case 2:
            now_serifu = 68;
            break;
        case 3:
            now_serifu = 69;
            break;
        case 4:
            goto_sub_sub_step(28);
            break;
        }
        break;
    case 22:
        serifu_run();
        break;
    case 23:
        v0 = get_g_zyotai_low(5);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = 8;
            break;
        case 2:
            now_serifu = 9;
            break;
        case 3:
            now_serifu = 10;
            break;
        }
        sub_sub_step_up();
        break;
    case 24:
        serifu_run();
        break;
    case 25:
        fade_down_main();
        break;
    case 26:
        now_serifu = 111;
        sub_sub_step_up();
        break;
    case 27:
        serifu_run();
        break;
    case 28:
        fade_out_step();
        break;
    case 29:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 30:
        serifu_run();
        break;
    case 31:
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
