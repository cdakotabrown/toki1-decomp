/* @SymbolStart: 0x80150284 */
/* @RodataBase: 0x80155A2C */
/* @SymbolSize: 600 */
/* Address: 0x8015028C */
/* Size: 592 bytes (148 insns) */
#include "types.h"

extern s32 sub_sub_step_up(void);
extern s32 goto_sub_sub_step(s32);
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
extern s32 bunkaken_sub_reset(void);
extern void bunkaken_basyo_init3(void);
extern void bunkaken_girl7_pair_bank_init(void);

extern u16 now_serifu;
extern u8 now_sub_sub_step;

void bunkaken_main6b(void) {
    s32 idx = now_sub_sub_step;
    s32 v0;

    switch (idx) {
    case 0:
        value_cache_find_or_load(16655, 1);
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
                now_serifu = 77;
                break;
            case 2:
                now_serifu = 80;
                break;
            case 3:
                now_serifu = 83;
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
        serifu_run();
        break;
    case 8:
        fade_up_main();
        break;
    case 9:
        serifu_run();
        break;
    case 10:
        v0 = get_g_zyotai_low(7);
        if ((u32)v0 < 2) {
            goto_sub_sub_step(12);
        } else {
            now_serifu = 21;
            fade_down_main();
        }
        break;
    case 11:
        serifu_run();
        break;
    case 12:
        v0 = get_g_zyotai_low(7);
        if ((u32)v0 < 2) {
            fade_out();
        } else {
            fade_out_step();
        }
        break;
    case 13:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 14:
        serifu_run();
        break;
    case 15:
        bunkaken_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
