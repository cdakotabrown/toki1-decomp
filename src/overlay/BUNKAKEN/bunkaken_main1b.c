/* @SymbolStart: 0x801341A4 */
/* @RodataBase: 0x801532F0 */
/* @SymbolSize: 604 */
/* Address: 0x801341AC */
/* Size: 596 bytes (149 insns) */
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
extern s32 fade_out(void);
extern s32 serifu_run(void);
extern s32 get_g_zyotai_low(s32);
extern void bunkaken_girl0_pair_bank_init(void);
extern s32 bunkaken_sub_reset(void);
extern void bunkaken_basyo_init3(void);

extern s16 now_serifu;
extern u8 now_sub_sub_step;

void bunkaken_main1b(void) {
    s32 t6 = now_sub_sub_step;
    s32 t;

    switch ((u32)t6) {
    case 0:
        value_cache_find_or_load(16655, 1);
        sub_sub_step_up();
        break;
    case 1:
        fade_out();
        break;
    case 2:
        if ((get_CD_State2() & 1) == 0) {
            break;
        }
        bunkaken_basyo_init3();
        t = dec_bg_init(1);
        indexed_byte_lsb_set(1, t);
        cd_read_start2(15, 0x801E8000, 18947);
        sub_sub_step_up();
        break;
    case 3:
        fade_in();
        break;
    case 4:
        if ((get_CD_State2() & 1) == 0) {
            break;
        }
        switch ((u32)get_g_zyotai_low(0)) {
        case 0:
        case 1:
            now_serifu = 92;
            break;
        case 2:
            now_serifu = 95;
            break;
        case 3:
            now_serifu = 98;
            break;
        }
        bunkaken_girl0_pair_bank_init();
        break;
    case 5:
        serifu_run();
        break;
    case 6:
        if (get_g_zyotai_low(0) == 3) {
            goto_sub_sub_step(12);
        } else {
            sub_sub_step_up();
        }
        break;
    case 7:
        fade_down_main();
        break;
    case 8:
        serifu_run();
        break;
    case 9:
        fade_up_main();
        break;
    case 10:
        if (get_g_zyotai_low(0) == 2) {
            now_serifu = 97;
        } else {
            now_serifu = 94;
        }
        sub_sub_step_up();
        break;
    case 11:
        serifu_run();
        break;
    case 12:
        fade_out();
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
