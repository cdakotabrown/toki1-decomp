/* @SymbolStart: 0x80137FB8 */
/* @RodataBase: 0x80153878 */
/* @SymbolSize: 904 */
/* Address: 0x80137FC0 */
/* Size: 896 bytes (224 insns) */
#include "types.h"

extern u8 now_sub_sub_step;

extern s32 sub_sub_step_up(void);
extern s32 goto_step(s32);
extern s32 get_CD_State2(void);
extern s32 cd_read_start2(s32, s32, s32);
extern s32 game_reset(void);
extern s32 set_dec_bri(s32);
extern s32 dec_bg_reset(void);
extern s32 indexed_byte_lsb_set(s32, s32);
extern s32 value_cache_find_or_load(s32, s32);
extern s32 dec_bg_init(s32);
extern s32 bustup_ptr_init(void);
extern s32 fade_in(void);
extern s32 fade_out(void);
extern s32 zyotai_sprite_set_chk(void);
extern s32 serifu_run(void);
extern s32 get_g_zyotai_low(s32);
extern s32 bustup_load_step(s32);
extern void bunkaken_girl4_pair_bank_init(void);
extern s32 bunkaken_sub_reset(void);
extern void bunkaken_basyo_init3(void);

extern s16 now_serifu;
extern u8  now_year;
extern u8  now_sub_step;
extern u8  now_girl;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 mes_base_y;

void bunkaken_main3e(void)
{
    s32 v0;

    switch (now_sub_sub_step) {
    case 0:
        value_cache_find_or_load(18044, 1);
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
            cd_read_start2(12, 0x801E8000, 19282);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(4);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 17;
                break;
            case 2:
                now_serifu = 18;
                break;
            case 3:
                now_serifu = 19;
                break;
            }
            bunkaken_girl4_pair_bank_init();
        }
        break;
    case 5:
        serifu_run();
        break;
    case 6:
        fade_out();
        break;
    case 7:
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_step(128);
        break;
    case 8:
        dec_bg_reset();
        set_dec_bri(0);
        value_cache_find_or_load(18044, 1);
        sub_sub_step_up();
        break;
    case 9:
        if (get_CD_State2() & 1) {
            bustup_load_step(26303);
        }
        break;
    case 10:
        bunkaken_basyo_init3();
        mes_base_y = 192;
        v0 = dec_bg_init(1);
        indexed_byte_lsb_set(1, v0);
        bustup_ptr_init();
        now_girl = 4;
        zyotai_sprite_set_chk();
        cd_read_start2(12, 0x801E8000, 19282);
        break;
    case 11:
        fade_in();
        break;
    case 12:
        if (get_CD_State2() & 1) {
            if (now_year == 95) {
                v0 = get_g_zyotai_low(4);
                switch (v0) {
                case 0:
                case 1:
                    now_serifu = 20;
                    break;
                case 2:
                    now_serifu = 21;
                    break;
                case 3:
                    now_serifu = 22;
                    break;
                }
            } else {
                v0 = get_g_zyotai_low(4);
                if ((u32)v0 < 2U) {
                    now_serifu = now_year * 2 - 169;
                } else {
                    now_serifu = now_year * 2 - 168;
                }
            }
            bunkaken_girl4_pair_bank_init();
        }
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
