/* @SymbolStart: 0x8013EC98 */
/* @RodataBase: 0x80154298 */
/* @SymbolSize: 788 */
/* Address: 0x8013ECA0 */
/* Size: 780 bytes (195 insns) */
#include "types.h"

extern s32 sub_sub_step_up(void);
extern s32 goto_sub_sub_step(s32);
extern s32 get_CD_State2(void);
extern s32 cd_read_start2(s32, s32, s32);
extern s32 game_reset(void);
extern s32 get_g_zyotai_h(s32);
extern s32 indexed_byte_lsb_set(s32, s32);
extern s32 value_cache_find_or_load(s32, s32);
extern s32 dec_bg_init(s32);
extern s32 fade_down_main(void);
extern s32 fade_in(void);
extern s32 fade_out_step(void);
extern s32 fade_out(void);
extern s32 bustup_face_change(s32);
extern s32 serifu_run(void);
extern s32 get_g_zyotai_low(s32);
extern void bunkaken_girl8_pair_bank_init(void);
extern s32 bunkaken_sub_reset(void);
extern void bunkaken_basyo_init3(void);

extern s16 now_serifu;
extern s16 now_serifu_in;
extern u8 now_year;
extern u8 now_sub_sub_step;

void bunkaken_girl8_pair_bk0_9596_exh(void) {
    s32 t6 = now_sub_sub_step;
    s32 v0;
    s32 a0;
    s32 t1;

    switch (t6) {
    case 0:
        value_cache_find_or_load(16618, 1);
        sub_sub_step_up();
        break;
    case 1:
        fade_out();
        break;
    case 2:
        v0 = get_CD_State2();
        if ((v0 & 1) == 0) {
            break;
        }
        bunkaken_basyo_init3();
        v0 = dec_bg_init(1);
        indexed_byte_lsb_set(1, v0);
        cd_read_start2(16, 0x801E8000, 18855);
        sub_sub_step_up();
        break;
    case 3:
        fade_in();
        break;
    case 4:
        v0 = get_CD_State2();
        if ((v0 & 1) == 0) {
            break;
        }
        v0 = get_g_zyotai_low(8);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = 2;
            break;
        case 2:
            now_serifu = 3;
            break;
        case 3:
            now_serifu = 4;
            break;
        }
        bunkaken_girl8_pair_bank_init();
        if (now_year == 96) {
            now_serifu = now_serifu + 3;
        }
        break;
    case 5:
        serifu_run();
        v0 = get_g_zyotai_low(8);
        if ((u32)v0 < 3) {
            v0 = now_serifu_in;
            if (v0 < 3) {
                break;
            }
            bustup_face_change(5);
        }
        break;
    case 6:
        v0 = get_g_zyotai_low(8);
        if (v0 == 3) {
            goto_sub_sub_step(9);
        } else {
            now_serifu = 1;
            fade_out();
        }
        break;
    case 7:
        serifu_run();
        bustup_face_change(3);
        break;
    case 8:
        fade_in();
        break;
    case 9:
        v0 = get_g_zyotai_low(8);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = 8;
            break;
        case 2:
            now_serifu = 9;
            break;
        case 3:
            now_serifu = 10;
            break;
        }
        sub_sub_step_up();
        break;
    case 10:
        serifu_run();
        v0 = now_serifu_in;
        if (v0 < 4) {
            break;
        }
        a0 = get_g_zyotai_h(8);
        bustup_face_change((u8)a0 & 0x7F);
        break;
    case 11:
        fade_down_main();
        break;
    case 12:
        fade_out_step();
        break;
    case 13:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 14:
        serifu_run();
        break;
    case 15:
        bunkaken_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
