/* @SymbolStart: 0x80149E68 */
/* @RodataBase: 0x8015E3E8 */
/* Address: 0x80149E70 */
/* Size: 828 bytes (207 insns) */
/* @SymbolSize: 836 */
#include "types.h"

extern u8 now_sub_sub_step;
extern void sub_sub_step_up(void);
extern void goto_sub_sub_step(s32);
extern s32 get_CD_State2(void);
extern void cd_read_start2(s32, s32, s32);
extern void game_reset(void);
extern void indexed_byte_lsb_set(s32, s32);
extern void value_cache_find_or_load(s32, s32);
extern s32 dec_bg_init(s32);
extern void bustup_cd_read(s32);
extern void bustup_ptr_init(void);
extern void fade_up_main(void);
extern void fade_down_main(void);
extern void fade_in_step(void);
extern void fade_out_step(void);
extern void zyotai_sprite_set_chk(void);
extern void serifu_run(void);
extern s32 get_g_zyotai_low(s32);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init(void);
extern void bunkasai_param_set(void);

extern s16 now_serifu;
extern u16 bukatu_pt4;
extern u8  now_girl;

void bunkasai_girl0_club4_95_scene(void) {
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
            bunkasai_basyo_init();
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            bustup_cd_read(21578);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in_step();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            bustup_ptr_init();
            now_girl = 0;
            zyotai_sprite_set_chk();
            now_serifu = 43;
        }
        break;
    case 5: case 9: case 11: case 13: case 15: case 19:
        serifu_run();
        break;
    case 6:
        cd_read_start2(17, 0x801E8000, 18879);
        sub_sub_step_up();
        break;
    case 7:
        fade_up_main();
        break;
    case 8:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(0);
            switch (v0) {

                break;
            case 0: case 1:
                now_serifu = 102;
                break;
            case 2:
                now_serifu = 106;
                break;
            case 3:
                now_serifu = 110;
                break;
            case 4:
                now_serifu = 114;
                break;
            }
            bunkasai_param_set();
        }
        break;
    case 10:
        v0 = get_g_zyotai_low(0);
        if (v0 == 4) {
            goto_sub_sub_step(16);
        } else {
            if ((u32)bukatu_pt4 < 100) {
                now_serifu = now_serifu + 1;
            }
            sub_sub_step_up();
        }
        break;
    case 12:
        v0 = get_g_zyotai_low(0);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = 105;
            break;
        case 2:
            now_serifu = 109;
            break;
        case 3:
            now_serifu = 113;
            break;
        }
        sub_sub_step_up();
        break;
    case 14:
        v0 = get_g_zyotai_low(0);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = 1;
            break;
        case 2:
            now_serifu = 118;
            break;
        case 3:
            now_serifu = 3;
            break;
        }
        sub_sub_step_up();
        break;
    case 16:
        fade_down_main();
        break;
    case 17:
        fade_out_step();
        break;
    case 18:
        now_serifu = 0;
        sub_sub_step_up();
        break;





    case 20:
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
