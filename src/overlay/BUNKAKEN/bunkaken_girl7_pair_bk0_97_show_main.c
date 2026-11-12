/* @SymbolStart: 0x8014EB30 */
/* @RodataBase: 0x80155820 */
/* @SymbolSize: 868 */
/* Address: 0x8014EB38 */
/* Size: 860 bytes (215 insns) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void goto_step(s32);
extern void goto_sub_sub_step(s32);
extern s32  get_CD_State2(void);
extern void cd_read_start2(s32, s32, s32);
extern void game_reset(void);
extern void indexed_byte_lsb_set(s32, s32);
extern void value_cache_find_or_load(s32, s32);
extern s32  dec_bg_init(s32);
extern void bustup_ptr_init(void);
extern void fade_down_main(void);
extern void fade_in(void);
extern void fade_out_step(void);
extern void fade_out(void);
extern void zyotai_sprite_set_chk(void);
extern void serifu_run(void);
extern s32  get_g_zyotai_low(s32);
extern void bustup_load_step(s32);
extern void bunkaken_sub_reset(void);
extern void bunkaken_basyo_init3(void);
extern void bunkaken_girl7_pair_bank_init(void);

extern s32 bunka_sd_kind;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 now_serifu;
extern s16 mes_base_y;
extern u8  now_girl;
extern u8  now_sub_step;
extern u8 now_sub_sub_step;

void bunkaken_girl7_pair_bk0_97_show_main(void)
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
            s32 ret;
            bunkaken_basyo_init3();
            ret = dec_bg_init(1);
            indexed_byte_lsb_set(1, ret);
            cd_read_start2(19, 0x801E8000, 19026);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(7);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 15;
                break;
            case 2:
                now_serifu = 16;
                break;
            case 3:
                now_serifu = 17;
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
        cd_read_start2(19, 0x801E8000, 19026);
        break;
    case 10:
        fade_in();
        break;
    case 11:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(7);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 18;
                break;
            case 2:
                now_serifu = 19;
                break;
            case 3:
                now_serifu = 20;
                break;
            }
            bunkaken_girl7_pair_bank_init();
        }
        break;
    case 12:
        serifu_run();
        break;
    case 13:
        v0 = get_g_zyotai_low(7);
        if ((u32)v0 < 2) {
            goto_sub_sub_step(16);
        } else {
            fade_down_main();
        }
        break;
    case 14:
        now_serifu = 21;
        sub_sub_step_up();
        break;
    case 15:
        serifu_run();
        break;
    case 16:
        v0 = get_g_zyotai_low(7);
        if ((u32)v0 < 2) {
            fade_out();
        } else {
            fade_out_step();
        }
        break;
    case 17:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 18:
        serifu_run();
        break;
    case 19:
        bunkaken_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
