/* @SymbolStart: 0x80138148 */
/* @SymbolSize: 576 */
/* @RodataBase: 0x8015C438 */
/* Address: 0x80138150 */
/* Size: 568 bytes (142 insns) */
#include "types.h"

extern void sub_sub_step_up(void);
extern s32 get_CD_State2(void);
extern void cd_read_start2(s32 a0, s32 a1, s32 a2);
extern void game_reset(void);
extern void indexed_byte_lsb_set(s32 a0, s32 a1);
extern void value_cache_find_or_load(s32 a0, s32 a1);
extern s32 dec_bg_init(s32 a0);
extern void bustup_cd_read(s32 a0);
extern void bustup_ptr_init(void);
extern void fade_up_main(void);
extern void fade_down_main(void);
extern void fade_in_step(void);
extern void fade_out_step(void);
extern void zyotai_sprite_set_chk(void);
extern void serifu_run(void);
extern s32 get_g_zyotai_low(s32 a0);
extern void bunkasai_girl4_srf_bank_set(void);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init(void);

extern s16 now_serifu;
extern u8 now_girl;

extern u8 now_sub_sub_step;
void bunkasai_girl4_club4_96_scene(void) {
    u32 t6 = now_sub_sub_step;

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
            bunkasai_basyo_init();
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            bustup_cd_read(26303);
            sub_sub_step_up();
        }
        break;

    case 3:
        fade_in_step();
        break;

    case 4:
        result = get_CD_State2();
        if (result & 1) {
            bustup_ptr_init();
            now_girl = 4;
            zyotai_sprite_set_chk();
            now_serifu = 44;
        }
        break;

    case 5:
        serifu_run();
        break;

    case 6:
        cd_read_start2(10, 0x801E8000, 19271);
        sub_sub_step_up();
        break;

    case 7:
        fade_up_main();
        break;

    case 8:
        result = get_CD_State2();
        if (result & 1) {
            v0 = get_g_zyotai_low(4);
            switch (v0) {
            case 0: case 1:
                now_serifu = 68;
                break;
            case 2: case 3:
                now_serifu = 70;
                break;
            case 4:
                now_serifu = 72;
                break;


                break;
            }
            bunkasai_girl4_srf_bank_set();
        }
        break;

    case 9:
        serifu_run();
        break;

    case 10:
        fade_down_main();
        break;

    case 11:
        serifu_run();
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
        bunkasai_sub_reset();
        break;

    default:
        game_reset();
        break;
    }
}
