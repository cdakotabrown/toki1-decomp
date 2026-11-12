/* @SymbolStart: 0x801483E8 */
/* @RodataBase: 0x80154F50 */
/* @SymbolSize: 492 */
/* Address: 0x801483F0 */
/* Size: 484 bytes (121 insns including 2-insn cross-prologue tail) */
#include "types.h"

extern s32 sub_sub_step_up(void);
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
extern void bunkaken_param_set(void);
extern s32 bunkaken_sub_reset(void);
extern void bunkaken_basyo_init3(void);

extern u16 now_serifu;
extern u8 now_sub_sub_step;

void bunkaken_main5e(void) {
    s32 idx = now_sub_sub_step;
    s32 v0;

    switch (idx) {
    case 0:
        value_cache_find_or_load(16655, 1);
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
            cd_read_start2(16, 0x801E8000, 19235);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(6);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 90;
                break;
            case 2:
                now_serifu = 93;
                break;
            case 3:
                now_serifu = 96;
                break;
            }
            bunkaken_param_set();
        }
        break;
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
        fade_out();
        break;
    case 11:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 12:
        serifu_run();
        break;
    case 13:
        bunkaken_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
