/* @SymbolStart: 0x80153260 */
/* @RodataBase: 0x8015F578 */
/* Address: 0x80153268 */
/* Size: 712 bytes (178 insns) */
/* @SymbolSize: 720 */
#include "types.h"

extern u8 now_sub_sub_step;
extern void sub_sub_step_up(void);
extern s32 get_CD_State2(void);
extern void cd_read_start2(s32, s32, s32);
extern void game_reset(void);
extern void indexed_byte_lsb_set(s32, s32);
extern void value_cache_find_or_load(s32, s32);
extern s32 dec_bg_init(s32);
extern void bustup_ptr_init(void);
extern void fade_down_main(void);
extern void fade_in(void);
extern void fade_out_step(void);
extern void fade_out(void);
extern void zyotai_sprite_set_chk(void);
extern void serifu_run(void);
extern s32 get_g_zyotai_low(s32);
extern void bustup_load_step(s32);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init(void);
extern void bunkasai_girl0_home_srf_bank_set(void);

extern s16 now_serifu;
extern u16 bukatu_pt4;
extern u8  now_girl;

void bunkasai_maina(void) {
    s32 idx;

    s32 v0;

    idx = now_sub_sub_step;
    switch (idx) {
    case 0:
        value_cache_find_or_load(16655, 1);
        sub_sub_step_up();
        break;
    case 1:
        fade_out_step();
        break;
    case 2:
        if (get_CD_State2() & 1) {
            bustup_load_step(21668);
        }
        break;
    case 3:
        v0 = dec_bg_init(1);
        indexed_byte_lsb_set(1, v0);
        bunkasai_basyo_init();
        bustup_ptr_init();
        now_girl = 0;
        zyotai_sprite_set_chk();
        cd_read_start2(24, 0x801E8000, 18896);
        break;
    case 4:
        fade_in();
        break;
    case 5:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(0);
            switch (v0) {

                break;
            case 0: case 1:
                now_serifu = 128;
                break;
            case 2:
                now_serifu = 129;
                break;
            case 3:
                now_serifu = 130;
                break;
            case 4:
                now_serifu = 131;
                break;
            }
            bunkasai_girl0_home_srf_bank_set();
        }
        break;
    case 6:
        serifu_run();
        break;
    case 7:
        now_serifu = 0;
        fade_out();
        break;
    case 8:
        serifu_run();
        break;
    case 9:
        fade_in();
        break;
    case 10:
        v0 = get_g_zyotai_low(0);
        switch (v0) {

            break;
        case 0: case 1:
            now_serifu = 132;
            break;
        case 2:
            now_serifu = 134;
            break;
        case 3:
            now_serifu = 134;
            break;
        case 4:
            now_serifu = 136;
            break;
        }
        if ((u32)bukatu_pt4 < 100) {
            now_serifu = now_serifu + 1;
        }
        sub_sub_step_up();
        break;
    case 11:
        serifu_run();
        break;
    case 12:
        fade_down_main();
        break;
    case 13:
        fade_out_step();
        break;
    case 14:
        now_serifu = 1;
        sub_sub_step_up();
        break;
    case 15:
        serifu_run();
        break;
    case 16:
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
