/* @SymbolStart: 0x801336AC */
/* @RodataBase: 0x80153208 */
/* @SymbolSize: 840 */
/* Address: 0x801336B4 */
/* Size: 832 bytes (208 insns + 2-insn cross-prologue tail) */
#include "types.h"

extern s32 sub_sub_step_up(void);
extern s32 goto_sub_step(s32);
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
extern s32 fade_out(void);
extern s32 serifu_run(void);
extern s32 get_g_zyotai_low(s32);
extern void bunkaken_girl0_pair_bank_init(void);
extern s32 bunkaken_sub_reset(void);
extern void bunkaken_basyo_init3(void);

extern s16 now_serifu;
extern s32 bunka_aisyou;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern u8  now_sub_step;
extern u8  now_sub_sub_step;

void bunkaken_girl0_pair_bk3_95_fort_main(void) {
    s32 idx = now_sub_sub_step;
    s32 v0;

    switch (idx) {
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
            cd_read_start2(15, 0x801E8000, 18947);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(0);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 59;
                break;
            case 2:
                now_serifu = 64;
                break;
            case 3:
                now_serifu = 69;
                break;
            }
            bunkaken_girl0_pair_bank_init();
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
        cd_read_start2(15, 0x801E8000, 18947);
        sub_sub_step_up();
        break;
    case 9:
        fade_up_main();
        break;
    case 10:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(0);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 60;
                break;
            case 2:
                now_serifu = 65;
                break;
            case 3:
                now_serifu = 70;
                break;
            }
            now_serifu = now_serifu + bunka_aisyou;
            bunkaken_girl0_pair_bank_init();
        }
        break;
    case 11:
        serifu_run();
        break;
    case 12:
        v0 = get_g_zyotai_low(0);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = 63;
            break;
        case 2:
            now_serifu = 68;
            break;
        case 3:
            now_serifu = 73;
            break;
        }
        sub_sub_step_up();
        break;
    case 13:
        serifu_run();
        break;
    case 14:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 15:
        fade_out();
        break;
    case 16:
        serifu_run();
        break;
    case 17:
        bunkaken_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
