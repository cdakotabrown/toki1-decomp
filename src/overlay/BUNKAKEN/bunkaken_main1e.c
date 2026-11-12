/* @SymbolStart: 0x8013491C */
/* @RodataBase: 0x801533A8 */
/* @SymbolSize: 768 */
/* Address: 0x80134924 */
/* Size: 760 bytes (190 insns) */
#include "types.h"

extern s32 sub_sub_step_up(void);
extern s32 goto_step(s32);
extern s32 get_CD_State2(void);
extern s32 cd_read_start2(s32, s32, s32);
extern s32 game_reset(void);
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
extern void bunkaken_girl0_pair_bank_init(void);
extern s32 bunkaken_sub_reset(void);
extern void bunkaken_basyo_init3(void);

extern s16 now_serifu;
extern u8 now_year;
extern u8 now_girl;
extern u8 now_sub_step;
extern s32 bunka_sd_kind;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 mes_base_y;
extern u8 now_sub_sub_step;

void bunkaken_main1e(void) {
    s32 t;

    switch ((u32)now_sub_sub_step) {
    case 0:
        value_cache_find_or_load(18044, 1);
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
            now_serifu = 118;
            break;
        case 2:
            now_serifu = 119;
            break;
        case 3:
            now_serifu = 120;
            break;
        }
        bunkaken_girl0_pair_bank_init();
        break;
    case 5:
        serifu_run();
        break;
    case 6:
        fade_out();
        break;
    case 7:
        bunka_sd_kind = 0;
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_step(128);
        break;
    case 8:
        bustup_load_step(21578);
        break;
    case 9:
        bunkaken_basyo_init3();
        mes_base_y = 192;
        bustup_ptr_init();
        now_girl = 0;
        zyotai_sprite_set_chk();
        cd_read_start2(15, 0x801E8000, 18947);
        break;
    case 10:
        fade_in();
        break;
    case 11:
        if ((get_CD_State2() & 1) == 0) {
            break;
        }
        switch ((u32)get_g_zyotai_low(0)) {
        case 0:
        case 1:
            now_serifu = 121;
            break;
        case 2:
            now_serifu = 122;
            break;
        case 3:
            now_serifu = 123;
            break;
        }
        if (now_year == 96) {
            now_serifu = now_serifu + 3;
        }
        bunkaken_girl0_pair_bank_init();
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
