/* @SymbolStart: 0x8014F254 */
/* @SymbolSize: 1040 */
/* @RodataBase: 0x801558C0 */
/* Address: 0x8014F25C (window starts at the absorbed cross-prologue head */
/* Size: 1032 bytes (258 insns) + 8-byte head = 1040 */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 goto_step();
extern s32 goto_sub_sub_step();
extern s32 get_CD_State2();
extern s32 cd_read_start2();
extern s32 game_reset();
extern s32 indexed_byte_lsb_set();
extern s32 value_cache_find_or_load();
extern s32 dec_bg_init();
extern s32 bustup_ptr_init();
extern s32 fade_up_main();
extern s32 fade_down_main();
extern s32 fade_in();
extern s32 fade_out_step();
extern s32 fade_out();
extern s32 zyotai_sprite_set_chk();
extern s32 serifu_run();
extern s32 get_g_zyotai_low();
extern s32 bustup_load_step();
extern s32 bunkaken_sub_reset();
extern void bunkaken_basyo_init3();
extern void bunkaken_girl7_pair_bank_init();

extern s16 now_serifu;
extern u8  now_year;
extern u8  now_girl;
extern u8  now_sub_step;
extern u8  now_sub_sub_step;
extern s32 bunka_sd_kind;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 mes_base_y;

void bunkaken_girl7_pair_bk2_show_main(void) {
    s32 sel = now_sub_sub_step;

    switch ((u32)sel) {
        case 0:
            value_cache_find_or_load(0x40FC, 1);
            sub_sub_step_up();
            break;

        case 1:
            fade_out();
            break;

        case 2:
            if (get_CD_State2() & 1) {
                bunkaken_basyo_init3();
                indexed_byte_lsb_set(1, dec_bg_init(1));
                cd_read_start2(19, 0x801E8000, 0x4A52);
                sub_sub_step_up();
            }
            break;

        case 3:
            fade_in();
            break;

        case 4:
            if (get_CD_State2() & 1) {
                switch (get_g_zyotai_low(7)) {
                    case 0:
                    case 1:
                        now_serifu = 34;
                        break;
                    case 2:
                        now_serifu = 35;
                        break;
                    case 3:
                        now_serifu = 36;
                        break;
                }
                bunkaken_girl7_pair_bank_init();
            }
            break;

        case 5:
            serifu_run();
            break;

        case 6:
            fade_out();
            break;

        case 7:
            bunka_sd_kind = 1;
            bunka_ret_sub_step = now_sub_step;
            bunka_ret_sub_sub_step = now_sub_sub_step + 1;
            goto_step(128);
            break;

        case 8:
            bustup_load_step(23333);
            break;

        case 9:
            bunkaken_basyo_init3();
            mes_base_y = 192;
            bustup_ptr_init();
            now_girl = 7;
            zyotai_sprite_set_chk();
            cd_read_start2(19, 0x801E8000, 0x4A52);
            break;

        case 10:
            fade_in();
            break;

        case 11:
            if (get_CD_State2() & 1) {
                s32 base;
                s32 mul;
                switch (get_g_zyotai_low(7)) {
                    case 0:
                    case 1:
                        now_serifu = 37;
                        break;
                    case 2:
                        now_serifu = 38;
                        break;
                    case 3:
                        now_serifu = 39;
                        break;
                }
                mul = now_year * 3;
                base = now_serifu;
                now_serifu = base + mul - 285;
                bunkaken_girl7_pair_bank_init();
            }
            break;

        case 12:
            serifu_run();
            break;

        case 13: {
            s32 ret;
            ret = get_g_zyotai_low(7);
            if ((u32)ret < 2U) {
                if (now_year == 95) {
                    goto_sub_sub_step(21);
                } else {
                    goto_sub_sub_step(17);
                }
            } else {
                sub_sub_step_up();
            }
            break;
        }

        case 14:
            sub_sub_step_up();
            break;

        case 15:
            now_serifu = 21;
            fade_down_main();
            break;

        case 16:
            serifu_run();
            break;

        case 17: {
            s32 ret;
            ret = get_g_zyotai_low(7);
            if ((u32)ret < 2U) {
                fade_out();
            } else {
                fade_out_step();
            }
            break;
        }

        case 18:
            now_serifu = 0;
            sub_sub_step_up();
            break;

        case 19:
            serifu_run();
            break;

        case 20:
            bunkaken_sub_reset();
            break;

        case 21:
            now_serifu = 46;
            fade_down_main();
            break;

        case 22:
            serifu_run();
            break;

        case 23:
            fade_up_main();
            break;

        case 24:
            serifu_run();
            break;

        case 25:
            goto_sub_sub_step(17);
            break;

        default:
            game_reset();
            break;
    }
}
