/* @SymbolStart: 0x801466FC */
/* @RodataBase: 0x8015DD84 */
/* Address: 0x80146704 */
/* Size: 884 bytes (221 insns) */
/* @SymbolSize: 892 */
#include "types.h"

extern u8 now_sub_sub_step;
extern void sub_sub_step_up(void);
extern void goto_sub_sub_step(s32);
extern s32 get_CD_State2(void);
extern void cd_read_start2(s32, s32, s32);
extern void game_reset(void);
extern s32 get_g_zyotai_h(s32);
extern void indexed_byte_lsb_set(s32, s32);
extern void value_cache_find_or_load(s32, s32);
extern s32 dec_bg_init(s32);
extern void bustup_cd_read(s32);
extern void bustup_ptr_init(void);
extern void fade_down_main(void);
extern void fade_in(void);
extern void fade_out_step(void);
extern void fade_out(void);
extern void bustup_face_change(s32);
extern void zyotai_sprite_set_chk(void);
extern void serifu_run(void);
extern s32 get_g_zyotai_low(s32);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init(void);
extern void bunkasai_girl3_home_srf_bank_set(void);

extern s16 now_serifu;
extern u16 bukatu_pt4;
extern u8  now_girl;

void bunkasai_girl3_home_club4_97_scene(void) {
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
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            bustup_cd_read(22748);
            sub_sub_step_up();
        }
        break;
    case 3:
        if (get_CD_State2() & 1) {
            bunkasai_basyo_init();
            bustup_ptr_init();
            now_girl = 3;
            zyotai_sprite_set_chk();
            cd_read_start2(12, 0x801E8000, 19196);
        }
        break;
    case 4:
        fade_in();
        break;
    case 5:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(3);
            switch (v0) {

                break;
            case 0: case 1:
                now_serifu = 16;
                break;
            case 2:
                now_serifu = 19;
                break;
            case 3:
                now_serifu = 20;
                break;
            case 4:
                now_serifu = 21;
                break;
            }
            bunkasai_girl3_home_srf_bank_set();
        }
        break;
    case 6:
        serifu_run();
        break;
    case 7:
        v0 = get_g_zyotai_low(3);
        if ((u32)v0 < 2) {
            bustup_face_change(0);
            sub_sub_step_up();
        } else {
            goto_sub_sub_step(11);
        }
        break;
    case 8:
        serifu_run();
        break;
    case 9:
        bustup_face_change(get_g_zyotai_h(3) & 0xFF);
        sub_sub_step_up();
        break;
    case 10:
        serifu_run();
        break;
    case 11:
        fade_out();
        break;
    case 12:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 13:
        serifu_run();
        break;
    case 14:
        fade_in();
        break;
    case 15:
        v0 = get_g_zyotai_low(3);
        switch (v0) {

            break;
        case 0: case 1:
            now_serifu = 22;
            break;
        case 2:
            now_serifu = 24;
            break;
        case 3:
            now_serifu = 26;
            break;
        case 4:
            now_serifu = 28;
            break;
        }
        if ((u32)bukatu_pt4 < 300) {
            now_serifu = now_serifu + 1;
        }
        sub_sub_step_up();
        break;
    case 16:
        serifu_run();
        break;
    case 17:
        fade_down_main();
        break;
    case 18:
        fade_out_step();
        break;
    case 19:
        now_serifu = 1;
        sub_sub_step_up();
        break;
    case 20:
        serifu_run();
        break;
    case 21:
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
