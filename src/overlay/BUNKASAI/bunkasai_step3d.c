/* @SymbolStart: 0x8013B688 */
/* @RodataBase: 0x8015CABC */
/* Address: 0x8013B690 */
/* Size: 1284 bytes (321 insns) */
/* @SymbolSize: 1292 */
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
extern void fade_in(void);
extern void fade_out_step(void);
extern void zyotai_sprite_set_chk(void);
extern void serifu_run(void);
extern s32 get_g_zyotai_low(s32);
extern void bunkasai_girl2_home_srf_bank_set(void);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init(void);

extern s16 now_serifu;
extern s16 girl2_syoushin;
extern u8  now_sub_step;
extern u8  now_sub_sub_step;
extern u8  now_girl;
extern s32 bunka_aisyou;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;

void bunkasai_step3d(void) {
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
            bustup_cd_read(21038);
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            sub_sub_step_up();
        }
        break;
    case 3:
        if (get_CD_State2() & 1) {
            bunkasai_basyo_init();
            bustup_ptr_init();
            now_girl = 2;
            zyotai_sprite_set_chk();
            cd_read_start2(10, 0x801E8000, 18988);
        }
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
                now_serifu = 30;
                break;
            case 3:
                now_serifu = 32;
                break;
            case 4:
                now_serifu = 34;
                break;

                break;
            }
            bunkasai_girl2_home_srf_bank_set();
            if (get_g_zyotai_low(2) == 0 && (girl2_syoushin & 1) == 0) {
                now_serifu = 52;
                goto_sub_sub_step(27);
            }
        }
        break;
    case 6:
        serifu_run();
        break;
    case 7:
        v0 = get_g_zyotai_low(2);
        if ((u32)v0 < 3) {
            sub_sub_step_up();
        } else {
            fade_down_main();
        }
        break;
    case 8:
        serifu_run();
        break;
    case 9:
        v0 = get_g_zyotai_low(2);
        if ((u32)v0 < 3) {
            fade_down_main();
        } else {
            sub_sub_step_up();
        }
        break;
    case 10:
        device_cmd_send(514);
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        sub_step_up();
        break;
    case 11:
        cd_read_start2(10, 0x801E8000, 18988);
        sub_sub_step_up();
        break;
    case 12:
        v0 = get_g_zyotai_low(2);
        if ((u32)v0 < 3) {
            fade_up_main();
        } else {
            sub_sub_step_up();
        }
        break;
    case 13:
        if (get_CD_State2() & 1) {
            bunkasai_girl2_home_srf_bank_set();
            v0 = get_g_zyotai_low(2);
            switch (v0) {

                break;
            case 0: case 1:
                now_serifu = bunka_aisyou * 2 + 36;
                break;
            case 2:
                now_serifu = bunka_aisyou * 2 + 42;
                break;
            case 3: case 4:
                now_serifu = bunka_aisyou + 48;
                goto_sub_sub_step(24);
                break;

                break;
            }
        }
        break;
    case 14:
        serifu_run();
        break;
    case 15:
        v0 = get_g_zyotai_low(2);
        if ((u32)v0 < 2 && bunka_aisyou != 1) {
            device_cmd_send(1288);
        }
        sub_sub_step_up();
        break;
    case 16:
        serifu_run();
        break;
    case 17:
        v0 = get_g_zyotai_low(2);
        if (v0 == 2) {
            now_serifu = 51;
            sub_sub_step_up();
        } else {
            goto_sub_sub_step(19);
        }
        break;
    case 18:
        serifu_run();
        break;
    case 19:
        fade_down_main();
        break;
    case 20:
        fade_out_step();
        break;
    case 21:
        now_serifu = 1;
        sub_sub_step_up();
        break;
    case 22:
        serifu_run();
        break;
    case 23:
        bunkasai_sub_reset();
        break;
    case 24:
        serifu_run();
        break;
    case 25:
        fade_out_step();
        break;
    case 26:
        goto_sub_sub_step(21);
        break;
    case 27:
        serifu_run();
        break;
    case 28:
        fade_down_main();
        break;
    case 29:
        serifu_run();
        break;
    case 30:
        fade_up_main();
        break;
    case 31:
        serifu_run();
        break;
    case 32:
        goto_sub_sub_step(19);
        break;
    default:
        game_reset();
        break;
    }
}
