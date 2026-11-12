/* @RodataBase: 0x80153490 */
/* @SymbolSize: 636 */
/* Address: 0x8013544C */
/* Size: 636 bytes (159 insns) */
#include "types.h"

extern s32 sub_sub_step_up(void);
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
extern void bunkaken_girl5_pair_bank_init(void);
extern s32 bunkaken_sub_reset(void);
extern void bunkaken_basyo_init3(void);

extern u16 now_serifu;
extern u8 now_sub_sub_step;

void bunkaken_main4a(void) {
    s32 idx;
    s32 v0;

    idx = now_sub_sub_step;

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
            cd_read_start2(16, 0x801E8000, 19059);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(5);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 5;
                break;
            case 2:
                now_serifu = 6;
                break;
            case 3:
                now_serifu = 7;
                break;
            }
            bunkaken_girl5_pair_bank_init();
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
        v0 = get_g_zyotai_low(5);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = 11;
            break;
        case 2:
            now_serifu = 13;
            break;
        case 3:
            now_serifu = 15;
            break;
        }
        fade_in();
        break;
    case 9:
        serifu_run();
        break;
    case 10:
        fade_out();
        break;
    case 11:
        fade_in();
        break;
    case 12:
        serifu_run();
        break;
    case 13:
        fade_out();
        break;
    case 14:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 15:
        serifu_run();
        break;
    case 16:
        bunkaken_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
