/* @SymbolStart: 0x80137A38 */
/* @SymbolSize: 672 */
/* @RodataBase: 0x801537F0 */
/* Address: 0x80137A40 */
/* Size: 672 bytes (168 insns) - includes 2-insn cross-prologue at tail */
#include "types.h"

extern s16 now_serifu;

extern s32 sub_sub_step_up();
extern s32 goto_sub_sub_step();
extern s32 get_CD_State2();
extern s32 cd_read_start2();
extern s32 game_reset();
extern s32 indexed_byte_lsb_set();
extern s32 value_cache_find_or_load();
extern s32 dec_bg_init();
extern s32 fade_in();
extern s32 fade_out();
extern s32 serifu_run();
extern s32 get_g_zyotai_low();
extern void bunkaken_girl4_pair_bank_init();
extern s32 bunkaken_sub_reset();
extern void bunkaken_basyo_init3();
extern u8 now_sub_sub_step;

void bunkaken_main3c(void) {
    s32 t6 = now_sub_sub_step;
    switch (t6) {
        case 0:
            value_cache_find_or_load(16618, 1);
            sub_sub_step_up();
            break;
        case 1:
            fade_out();
            break;
        case 2: {
            s32 r = get_CD_State2();
            if (!(r & 1)) break;
            bunkaken_basyo_init3();
            r = dec_bg_init(1);
            indexed_byte_lsb_set(1, r);
            cd_read_start2(12, 0x801E8000, 19282);
            sub_sub_step_up();
            break;
        }
        case 3:
            fade_in();
            break;
        case 4: {
            s32 r = get_CD_State2();
            if (!(r & 1)) break;
            r = get_g_zyotai_low(4);
            switch (r) {
                case 0:
                case 1:
                    now_serifu = 6;
                    break;
                case 2:
                    now_serifu = 7;
                    break;
                case 3:
                    now_serifu = 8;
                    break;
                default:
                    goto L_after_store;
            }
            L_after_store:
            bunkaken_girl4_pair_bank_init();
            break;
        }
        case 5:
            serifu_run();
            break;
        case 6: {
            s32 t2 = 1;
            now_serifu = t2;
            fade_out();
            break;
        }
        case 7:
            serifu_run();
            break;
        case 8: {
            s32 r = get_g_zyotai_low(4);
            switch (r) {
                case 0:
                case 1:
                    now_serifu = 9;
                    break;
                case 2:
                    now_serifu = 10;
                    break;
                case 3:
                    now_serifu = 11;
                    break;
                default:
                    goto L_after2;
            }
            L_after2:
            fade_in();
            break;
        }
        case 9:
            serifu_run();
            break;
        case 10: {
            s32 r = get_g_zyotai_low(4);
            if (r == 3) {
                fade_out();
            } else {
                goto_sub_sub_step(13);
            }
            break;
        }
        case 11:
            fade_in();
            break;
        case 12:
            serifu_run();
            break;
        case 13:
            now_serifu = 0;
            sub_sub_step_up();
            break;
        case 14:
            fade_out();
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
