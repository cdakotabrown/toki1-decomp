/* @SymbolStart: 0x80137304 */
/* @RodataBase: 0x8015C268 */
/* Address: 0x8013730C */
/* Size: 1148 bytes (287 insns) */
/* @SymbolSize: 1156 */
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
extern void bunkasai_girl4_srf_bank_set(void);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init(void);

extern s16 now_serifu;
extern u8  now_sub_step;
extern u8  now_sub_sub_step;
extern u8  now_girl;
extern s32 bunka_aisyou;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;

void bunkasai_main5a(void) {
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
            bustup_cd_read(26303);
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
            now_girl = 4;
            zyotai_sprite_set_chk();
        }
        break;
    case 5:
        serifu_run();
        break;
    case 6:
        cd_read_start2(10, 0x801E8000, 19271);
        sub_sub_step_up();
        break;
    case 7:
        fade_up_main();
        break;
    case 8:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(4);
            switch (v0) {

                break;
            case 0: case 1:
                now_serifu = 32;
                break;
            case 2: case 3:
                now_serifu = 33;
                break;
            case 4:
                now_serifu = 34;
                break;

                break;
            }
            bunkasai_girl4_srf_bank_set();
        }
        break;
    case 9:
        serifu_run();
        break;
    case 10:
        fade_down_main();
        break;
    case 11:
        v0 = get_g_zyotai_low(4);
        if (v0 == 4) {
            serifu_run();
        } else {
            sub_sub_step_up();
        }
        break;
    case 12:
        device_cmd_send(514);
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        sub_step_up();
        break;
    case 13:
        cd_read_start2(10, 0x801E8000, 19271);
        sub_sub_step_up();
        break;
    case 14:
        v0 = get_g_zyotai_low(4);
        if (v0 == 4) {
            sub_sub_step_up();
        } else {
            fade_up_main();
        }
        break;
    case 15:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(4);
            switch (v0) {

                break;
            case 0: case 1:
                now_serifu = 36;
                break;
            case 2: case 3:
                now_serifu = 39;
                break;
            case 4:
                now_serifu = 42;
                break;

                break;
            }
            now_serifu = now_serifu + bunka_aisyou;
            bunkasai_girl4_srf_bank_set();
        }
        break;
    case 16:
        serifu_run();
        break;
    case 17:
        sub_sub_step_up();
        v0 = get_g_zyotai_low(4);
        switch (v0) {

            break;
        case 0: case 1:
            now_serifu = 45;
            break;
        case 2: case 3:
            now_serifu = 46;
            break;
        case 4:
            goto_sub_sub_step(22);
            break;

            break;
        }
        break;
    case 18:
        serifu_run();
        break;
    case 19:
        v0 = get_g_zyotai_low(4);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = 1;
            break;
        case 2:
        case 3:
            now_serifu = 2;
            break;
        }
        sub_sub_step_up();
        break;
    case 20:
        serifu_run();
        break;
    case 21:
        fade_down_main();
        break;
    case 22:
        fade_out_step();
        break;
    case 23:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 24:
        serifu_run();
        break;
    case 25:
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
