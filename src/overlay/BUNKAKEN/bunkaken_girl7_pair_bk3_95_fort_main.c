/* @SymbolStart: 0x8014F664 */
/* @RodataBase: 0x80155928 */
/* @SymbolSize: 948 */
/* Address: 0x8014F66C */
/* Size: 948 bytes (237 insns; last 2 are next func's cross-prologue) */
#include "types.h"

extern s32 sub_sub_step_up(void);
extern s32 goto_sub_step(s32);
extern s32 goto_sub_sub_step(s32);
extern s32 device_cmd_send(s32);
extern s32 get_CD_State2(void);
extern s32 cd_read_start2(s32, s32, s32);
extern s32 game_reset(void);
extern s32 indexed_byte_lsb_set(s32, s32);
extern s32 value_cache_find_or_load(s32, s32);
extern s32 dec_bg_init(s32);
extern s32 fade_up_main(void);
extern s32 fade_down_main(void);
extern s32 fade_in(void);
extern s32 fade_out_step(void);
extern s32 fade_out(void);
extern s32 serifu_run(void);
extern s32 get_g_zyotai_low(s32);
extern s32 bunkaken_basyo_init3(void);
extern s32 bunkaken_sub_reset(void);
extern void bunkaken_girl7_pair_bank_init(void);

extern s16 now_serifu;
extern u8  now_sub_step;
extern u8  now_sub_sub_step;
extern s32 bunka_aisyou;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;

void bunkaken_girl7_pair_bk3_95_fort_main(void) {
    s32 idx = now_sub_sub_step;
    s32 v0;

    switch ((u32)idx) {
    case 0:
        value_cache_find_or_load(16645, 1);
        sub_sub_step_up();
        break;
    case 1:
        fade_out();
        break;
    case 2:
        if (get_CD_State2() & 1) {
            bunkaken_basyo_init3();
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            cd_read_start2(19, 0x801E8000, 19026);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(7);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 48;
                break;
            case 2:
                now_serifu = 49;
                break;
            case 3:
                now_serifu = 50;
                break;
            }
            bunkaken_girl7_pair_bank_init();
        }
        break;
    case 5:
        serifu_run();
        break;
    case 6:
        fade_down_main();
        break;
    case 7:
        device_cmd_send(514);
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_sub_step(11);
        break;
    case 8:
        cd_read_start2(19, 0x801E8000, 19026);
        sub_sub_step_up();
        break;
    case 9:
        fade_up_main();
        break;
    case 10:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(7);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 51;
                break;
            case 2:
                now_serifu = 54;
                break;
            case 3:
                now_serifu = 57;
                break;
            }
            now_serifu = now_serifu + bunka_aisyou;
            bunkaken_girl7_pair_bank_init();
        }
        break;
    case 11:
        serifu_run();
        break;
    case 12:
        v0 = get_g_zyotai_low(7);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = 60;
            break;
        case 2:
            now_serifu = 61;
            break;
        case 3:
            now_serifu = 62;
            break;
        }
        sub_sub_step_up();
        break;
    case 13:
        serifu_run();
        break;
    case 14:
        v0 = get_g_zyotai_low(7);
        if ((u32)v0 < 2) {
            goto_sub_sub_step(16);
        } else {
            now_serifu = 21;
            fade_down_main();
        }
        break;
    case 15:
        serifu_run();
        break;
    case 16:
        v0 = get_g_zyotai_low(7);
        if ((u32)v0 < 2) {
            fade_out();
        } else {
            fade_out_step();
        }
        break;
    case 17:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 18:
        serifu_run();
        break;
    case 19:
        bunkaken_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
