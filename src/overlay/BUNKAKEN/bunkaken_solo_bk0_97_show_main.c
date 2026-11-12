/* @SymbolStart: 0x8013A118 */
/* @RodataBase: 0x80153B90 */
/* @SymbolSize: 484 */
/* Address: 0x8013A120 */
/* Size: 484 bytes (121 insns including 2-insn cross-prologue tail) */
#include "types.h"

extern u8 now_sub_sub_step;

extern s32 sub_sub_step_up(void);
extern s32 goto_step(s32);
extern s32 get_CD_State2(void);
extern s32 cd_read_start2(s32, s32, s32);
extern s32 game_reset(void);
extern s32 indexed_byte_lsb_set(s32, s32);
extern s32 value_cache_find_or_load(s32, s32);
extern s32 dec_bg_init(s32);
extern s32 fade_in_step(void);
extern s32 fade_out_step(void);
extern s32 serifu_run(void);
extern void bunkaken_common_bank_init(void);
extern s32 bunkaken_sub_reset(void);
extern void bunkaken_basyo_init3(void);

extern s16 now_serifu;
extern u8  now_sub_step;
extern s32 bunka_sd_kind;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 mes_base_y;

void bunkaken_solo_bk0_97_show_main(void)
{
    s32 v0;

    switch (now_sub_sub_step) {
    case 0:
        value_cache_find_or_load(18044, 1);
        sub_sub_step_up();
        break;
    case 1:
    case 6:
    case 12:
        fade_out_step();
        break;
    case 2:
        if (get_CD_State2() & 1) {
            bunkaken_basyo_init3();
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            cd_read_start2(5, 0x801E8000, 18874);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in_step();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            now_serifu = 54;
            bunkaken_common_bank_init();
        }
        break;
    case 5:
    case 11:
    case 14:
        serifu_run();
        break;
    case 7:
        bunka_sd_kind = 4;
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_step(128);
        break;
    case 8:
        bunkaken_basyo_init3();
        mes_base_y = 192;
        cd_read_start2(5, 0x801E8000, 18874);
        sub_sub_step_up();
        break;
    case 9:
        fade_in_step();
        break;
    case 10:
        if (get_CD_State2() & 1) {
            now_serifu = 55;
            bunkaken_common_bank_init();
        }
        break;
    case 13:
        now_serifu = 82;
        sub_sub_step_up();
        break;
    case 15:
        bunkaken_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
