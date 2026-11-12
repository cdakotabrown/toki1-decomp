/* @RodataBase: 0x80153360 */
/* @SymbolSize: 848 */
/* Address: 0x801345CC */
/* Size: 848 bytes (212 insns including 2-insn cross-prologue tail) */
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
extern void bunkaken_girl0_pair_bank_init(void);
extern s32 bunkaken_sub_reset(void);
extern void bunkaken_basyo_init3(void);

extern s16 now_serifu;
extern s16 para_gei;
extern u8 now_sub_sub_step;

void bunkaken_main1d(void) {
    s32 idx;
    s32 v0;

    idx = now_sub_sub_step;

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
                now_serifu = 105;
                break;
            case 2:
                now_serifu = 110;
                break;
            case 3:
                now_serifu = 113;
                break;
            }
            bunkaken_girl0_pair_bank_init();
        }
        break;
    case 5:
        serifu_run();
        break;
    case 6:
        now_serifu = 137;
        fade_out();
        break;
    case 7:
        serifu_run();
        break;
    case 8:
        v0 = get_g_zyotai_low(0);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = 106;
            break;
        case 2:
            now_serifu = 111;
            break;
        case 3:
            now_serifu = 114;
            break;
        }
        fade_in();
        break;
    case 9:
        serifu_run();
        break;
    case 10:
        v0 = get_g_zyotai_low(0);
        if (v0 == 3) {
            if (para_gei >= 120) {
                now_serifu = 115;
            } else {
                now_serifu = 116;
            }
        } else {
            if (para_gei >= 120) {
                now_serifu = 107;
            } else {
                now_serifu = 108;
            }
        }
        sub_sub_step_up();
        break;
    case 11:
        serifu_run();
        break;
    case 12:
        v0 = get_g_zyotai_low(0);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = 109;
            break;
        case 2:
            now_serifu = 112;
            break;
        case 3:
            now_serifu = 117;
            break;
        }
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
