/* @SymbolStart: 0x80132774 */
/* @RodataBase: 0x801530A8 */
/* Address: 0x8013277C */
/* Size: 848 bytes (212 insns) */
/* @SymbolSize: 848 */
#include "types.h"

extern void sub_sub_step_up(void);
extern s32 get_CD_State2(void);
extern void cd_read_start2(s32, s32, s32);
extern void game_reset(void);
extern void indexed_byte_lsb_set(s32, s32);
extern void value_cache_find_or_load(s32, s32);
extern s32 dec_bg_init(s32);
extern void fade_in(void);
extern void fade_out(void);
extern void serifu_run(void);
extern s32 get_g_zyotai_low(s32);
extern void bunkaken_girl0_pair_bank_init(void);
extern void bunkaken_sub_reset(void);
extern void bunkaken_basyo_init3(void);

extern s16 now_serifu;
extern s16 para_bun;
extern u8 now_sub_sub_step;

void bunkaken_girl0_pair_bk0_95_exh_main(void) {
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
                now_serifu = 1;
                break;
            case 2:
                now_serifu = 6;
                break;
            case 3:
                now_serifu = 9;
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
            now_serifu = 2;
            break;
        case 2:
            now_serifu = 7;
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
        v0 = get_g_zyotai_low(0);
        if (v0 == 3) {
            if (para_bun >= 120) {
                now_serifu = 11;
            } else {
                now_serifu = 12;
            }
        } else {
            if (para_bun >= 120) {
                now_serifu = 3;
            } else {
                now_serifu = 4;
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
            now_serifu = 5;
            break;
        case 2:
            now_serifu = 8;
            break;
        case 3:
            now_serifu = 13;
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
