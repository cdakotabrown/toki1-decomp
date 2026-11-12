/* @SymbolStart: 0x8013A95C */
/* @SymbolSize: 580 */
/* @RodataBase: 0x8015C924 */
/* Address: 0x8013A964 */
/* Size: 572 bytes (143 insns) */
#include "types.h"

extern void sub_sub_step_up(void);
extern s32 get_CD_State2(void);
extern void cd_read_start2(s32 a0, s32 a1, s32 a2);
extern void game_reset(void);
extern void indexed_byte_lsb_set(s32 a0, s32 a1);
extern void value_cache_find_or_load(s32 a0, s32 a1);
extern s32 dec_bg_init(s32 a0);
extern void bustup_ptr_init(void);
extern void fade_up_main(void);
extern void fade_down_main(void);
extern void fade_in_step(void);
extern void fade_out_step(void);
extern void zyotai_sprite_set_chk(void);
extern void serifu_run(void);
extern s32 get_g_zyotai_low(s32 a0);
extern void bustup_load_step(s32 a0);
extern void bunkasai_girl8_srf_bank_set(void);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init(void);

extern s16 now_serifu;
extern u8 now_girl;

extern u8 now_sub_sub_step;
void bunkasai_step8e(void) {
    s32 t6 = now_sub_sub_step;

    s32 result;
    s32 v0;

    switch (t6) {
    case 0:
        value_cache_find_or_load(16664, 1);
        sub_sub_step_up();
        break;

    case 1:
        fade_out_step();
        break;

    case 2:
        result = get_CD_State2();
        if (result & 1) {
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            sub_sub_step_up();
        }
        break;

    case 3:
        bustup_load_step(20498);
        break;

    case 4:
        bustup_ptr_init();
        now_girl = 8;
        zyotai_sprite_set_chk();
        bunkasai_basyo_init();
        now_serifu = 44;
        break;

    case 5:
        fade_in_step();
        break;

    case 6:
        serifu_run();
        break;

    case 7:
        cd_read_start2(20, 0x801E8000, 18833);
        sub_sub_step_up();
        break;

    case 8:
        fade_up_main();
        break;

    case 9:
        result = get_CD_State2();
        if (result & 1) {
            v0 = get_g_zyotai_low(8);
            switch (v0) {
            case 0: case 1:
                now_serifu = 97;
                break;
            case 2:
                now_serifu = 99;
                break;
            case 3:
                now_serifu = 101;
                break;
            case 4:
                now_serifu = 103;
                break;

                break;
            }
            bunkasai_girl8_srf_bank_set();
        }
        break;

    case 10:
        serifu_run();
        break;

    case 11:
        fade_down_main();
        break;

    case 12:
        serifu_run();
        break;

    case 13:
        fade_out_step();
        break;

    case 14:
        now_serifu = 0;
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
