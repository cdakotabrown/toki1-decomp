/* @SymbolStart: 0x8013A2FC */
/* @RodataBase: 0x80153BD0 */
/* Address: 0x8013A304; .sym window 540 bytes starting at the hoisted */
/* @SymbolSize: 540 */
#include "types.h"

extern volatile s16 now_serifu;
extern volatile s16 mes_base_y;
extern volatile s32 bunka_ret_sub_step;
extern volatile s32 bunka_ret_sub_sub_step;
extern volatile u8 now_sub_step;

extern void sub_sub_step_up(void);
extern s32 goto_step(s32);
extern s32 get_CD_State2(void);
extern void cd_read_start2(s32, u32, s32);
extern void game_reset(void);
extern void dec_bg_reset(void);
extern void set_dec_bri(s32);
extern void indexed_byte_lsb_set(s32, s32);
extern s32 dec_bg_init(s32);
extern void value_cache_find_or_load(s32, s32);
extern void fade_in_step(void);
extern void fade_out_step(void);
extern void serifu_run(void);
extern void bunkaken_common_bank_init(void);
extern void bunkaken_basyo_init3(void);
extern void bunkaken_sub_reset(void);
extern u8 now_sub_sub_step;

void bunkaken_solo_bk1_show_main(void)
{
    switch (now_sub_sub_step) {
    case 0:
        value_cache_find_or_load(18044, 1);
        sub_sub_step_up();
        break;
    case 1:
    case 6:
    case 13:
        fade_out_step();
        break;
    case 2:
        if (get_CD_State2() & 1) {
            bunkaken_basyo_init3();
            indexed_byte_lsb_set(1, dec_bg_init(1));
            cd_read_start2(5, 0x801E8000U, 18874);
            sub_sub_step_up();
        }
        break;
    case 3:
    case 10:
        fade_in_step();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            now_serifu = 56;
            bunkaken_common_bank_init();
        }
        break;
    case 5:
    case 12:
    case 15:
        serifu_run();
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
            bunkaken_basyo_init3();
            mes_base_y = 192;
            indexed_byte_lsb_set(1, dec_bg_init(1));
            cd_read_start2(5, 0x801E8000U, 18874);
            sub_sub_step_up();
        }
        break;
    case 11:
        if (get_CD_State2() & 1) {
            now_serifu = 57;
            bunkaken_common_bank_init();
        }
        break;
    case 14:
        now_serifu = 82;
        sub_sub_step_up();
        break;
    case 16:
        bunkaken_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
