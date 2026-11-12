/* @SymbolStart: 0x80146740 */
/* @RodataBase: 0x80154CC8 */
/* @SymbolSize: 732 */
/* Address: 0x80146748 */
/* Size: 724 bytes (181 insns) */
#include "types.h"

extern s32 sub_sub_step_up(void);
extern s32 goto_sub_sub_step(s32);
extern s32 get_CD_State2(void);
extern s32 cd_read_start2(s32, s32, s32);
extern s32 game_reset(void);
extern s32 indexed_byte_lsb_set(s32, s32);
extern s32 value_cache_find_or_load(s32, s32);
extern s32 dec_bg_init(s32);
extern s32 fade_in(void);
extern s32 fade_out(void);
extern s32 serifu_run(void);
extern s32 get_g_zyotai_low(s32);
extern s32 bunkaken_sub_reset(void);
extern void bunkaken_basyo_init3(void);
extern void bunkaken_param_set(void);

extern s16 now_serifu;
extern s16 para_bun;
extern u8 now_sub_sub_step;

void bunkaken_girl6_pair_bk0_95_exh_main(void) {
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
            cd_read_start2(16, 0x801E8000, 19235);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(6);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 2;
                break;
            case 2:
                now_serifu = 3;
                break;
            case 3:
                now_serifu = 4;
                break;
            }
            bunkaken_param_set();
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
        v0 = get_g_zyotai_low(6);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = 5;
            break;
        case 2:
            now_serifu = 9;
            break;
        case 3:
            now_serifu = 10;
            break;
        }
        fade_in();
        break;
    case 9:
        serifu_run();
        break;
    case 10:
        v0 = get_g_zyotai_low(6);
        if ((u32)v0 < 2) {
            if (para_bun < 120) {
                now_serifu = now_serifu + 1;
            }
            sub_sub_step_up();
        } else {
            goto_sub_sub_step(14);
        }
        break;
    case 11:
        serifu_run();
        break;
    case 12:
        now_serifu = 8;
        sub_sub_step_up();
        break;
    case 13:
        serifu_run();
        break;
    case 14:
        fade_out();
        break;
    case 15:
        now_serifu = 0;
        sub_sub_step_up();
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
