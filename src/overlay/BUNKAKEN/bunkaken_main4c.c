/* @SymbolStart: 0x801359AC */
/* @SymbolSize: 852 */
/* @RodataBase: 0x80153518 */
/* Address: 0x801359B4 (the window starts at the absorbed 2-insn */
/* Size: 852 bytes (213 insns including the 2-insn cross-prologue head) */
#include "types.h"

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
extern void bunkaken_girl5_pair_bank_init(void);
extern s32 bunkaken_sub_reset(void);
extern void bunkaken_basyo_init3(void);

extern s16 now_serifu;
extern u8  now_year;
extern u8  now_sub_step;
extern u8  now_girl;
extern u8  now_sub_sub_step;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 mes_base_y;

void bunkaken_main4c(void) {
    s32 idx = now_sub_sub_step;
    s32 v0;

    switch (idx) {
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
                now_serifu = 23;
                break;
            case 2:
                now_serifu = 24;
                break;
            case 3:
                now_serifu = 25;
                break;
            }
            bunkaken_girl5_pair_bank_init();
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
            bustup_load_step(24503);
        }
        break;
    case 10:
        bunkaken_basyo_init3();
        mes_base_y = 192;
        v0 = dec_bg_init(1);
        indexed_byte_lsb_set(1, v0);
        bustup_ptr_init();
        now_girl = 5;
        zyotai_sprite_set_chk();
        cd_read_start2(16, 0x801E8000, 19059);
        break;
    case 11:
        fade_in();
        break;
    case 12:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(5);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 26;
                break;
            case 2:
                now_serifu = 27;
                break;
            case 3:
                now_serifu = 28;
                break;
            }
            now_serifu = now_serifu + 3 * now_year - 285;
            bunkaken_girl5_pair_bank_init();
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
