/* @SymbolStart: 0x801404B8 */
/* @RodataBase: 0x80154498 */
/* @SymbolSize: 508 */
/* Address: 0x801404C0 */
/* Size: 500 bytes (125 insns) */
#include "types.h"

extern s16 now_serifu;

extern s32 sub_sub_step_up();
extern s32 get_CD_State2();
extern s32 cd_read_start2();
extern s32 game_reset();
extern s32 indexed_byte_lsb_set();
extern s32 value_cache_find_or_load();
extern s32 dec_bg_init();
extern s32 fade_up_main();
extern s32 fade_down_main();
extern s32 fade_in();
extern s32 fade_out_step();
extern s32 fade_out();
extern s32 serifu_run();
extern s32 get_g_zyotai_low();
extern void bunkaken_girl8_pair_bank_init();
extern s32 bunkaken_sub_reset();
extern void bunkaken_basyo_init3();
extern u8 now_sub_sub_step;

void bunkaken_girl8_pair_bk4_95_exh_main(void) {
    s32 t6 = now_sub_sub_step;
    switch (t6) {
        case 0:
            value_cache_find_or_load(16655, 1);
            sub_sub_step_up();
            break;
        case 1:
            fade_out();
            break;
        case 2: {
            s32 v0 = get_CD_State2();
            if (!(v0 & 1)) break;
            bunkaken_basyo_init3();
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            cd_read_start2(16, 0x801E8000, 18855);
            sub_sub_step_up();
            break;
        }
        case 3:
            fade_in();
            break;
        case 4: {
            s32 v0 = get_CD_State2();
            if (!(v0 & 1)) break;
            v0 = get_g_zyotai_low(8);
            switch (v0) {
                case 0:
                case 1:
                    now_serifu = 87;
                    break;
                case 2:
                    now_serifu = 90;
                    break;
                case 3:
                    now_serifu = 93;
                    break;
            }
            bunkaken_girl8_pair_bank_init();
            break;
        }
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
            fade_down_main();
            break;
        case 11:
            fade_out_step();
            break;
        case 12:
            now_serifu = 0;
            sub_sub_step_up();
            break;
        case 13:
            serifu_run();
            break;
        case 14:
            bunkaken_sub_reset();
            break;
        default:
            game_reset();
            break;
    }
}
