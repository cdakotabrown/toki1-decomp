/* @SymbolStart: 0x8014E620 */
/* @RodataBase: 0x80155778 */
/* @SymbolSize: 688 */
/* Address: 0x8014E628 */
/* Size: 680 bytes (170 insns) */
#include "types.h"

extern s32 sub_sub_step_up(void);
extern s32 goto_sub_sub_step(s32);
extern s32 get_CD_State2(void);
extern s32 cd_read_start2(s32, s32, s32);
extern s32 game_reset(void);
extern s32 indexed_byte_lsb_set(s32, s32);
extern s32 value_cache_find_or_load(s32, s32);
extern s32 dec_bg_init(s32);
extern s32 fade_down_main(void);
extern s32 fade_in(void);
extern s32 fade_out_step(void);
extern s32 fade_out(void);
extern s32 serifu_run(void);
extern s32 get_g_zyotai_low(s32);
extern s32 bunkaken_sub_reset(void);
extern void bunkaken_basyo_init3(void);
extern void bunkaken_girl7_pair_bank_init(void);

extern s16 now_serifu;
extern u8 now_sub_sub_step;

void bunkaken_girl7_pair_bk0_95_exh_main(void) {
    s32 idx = now_sub_sub_step;
    s32 v0;

    switch (idx) {
    case 0:
        value_cache_find_or_load(16618, 1);
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
            bunkaken_girl7_pair_bank_init();
            v0 = get_g_zyotai_low(7);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 3;
                break;
            case 2:
                now_serifu = 5;
                goto_sub_sub_step(14);
                break;
            case 3:
                now_serifu = 8;
                goto_sub_sub_step(18);
                break;
            }
        }
        break;
    case 5:
        serifu_run();
        break;
    case 6:
        now_serifu = 1;
        fade_out();
        break;
    case 7:
        serifu_run();
        break;
    case 8:
        now_serifu = 4;
        fade_in();
        break;
    case 9:
        serifu_run();
        break;
    case 10:
        fade_out();
        break;
    case 11:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 12:
        serifu_run();
        break;
    case 13:
        bunkaken_sub_reset();
        break;
    case 14:
        serifu_run();
        break;
    case 15:
        now_serifu = 2;
        fade_out();
        break;
    case 16:
        serifu_run();
        break;
    case 17:
        now_serifu = 6;
        fade_in();
        break;
    case 18:
        serifu_run();
        break;
    case 19:
        fade_down_main();
        break;
    case 20:
        serifu_run();
        break;
    case 21:
        fade_out_step();
        break;
    case 22:
        goto_sub_sub_step(11);
        break;
    default:
        game_reset();
        break;
    }
}
