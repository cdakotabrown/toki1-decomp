/* @SymbolStart: 0x80145334 */
/* @RodataBase: 0x8015DB24 */
/* Address: 0x8014533C */
/* Size: 932 bytes (233 insns) */
/* @SymbolSize: 940 */
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
extern void fade_up_main(void);
extern void fade_down_main(void);
extern void fade_in_step(void);
extern void fade_out_step(void);
extern void bustup_face_change(s32);
extern void zyotai_sprite_set_chk(void);
extern void serifu_run(void);
extern s32 get_g_zyotai_low(s32);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init(void);
extern void bunkasai_girl7_srf_bank_set(void);

extern s16 now_serifu;
extern s16 now_serifu_in;
extern u16 bukatu_pt4;
extern u8  now_girl;

void bunkasai_girl7_club4_95_scene(void) {
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
            bustup_cd_read(23333);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in_step();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            bustup_ptr_init();
            now_girl = 7;
            zyotai_sprite_set_chk();
            now_serifu = 43;
        }
        break;
    case 5:
        serifu_run();
        break;
    case 6:
        cd_read_start2(15, 0x801E8000, 19009);
        sub_sub_step_up();
        break;
    case 7:
        fade_up_main();
        break;
    case 8:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(7);
            switch (v0) {

                break;
            case 0: case 1:
                now_serifu = 74;
                break;
            case 2:
                now_serifu = 77;
                break;
            case 3:
                now_serifu = 80;
                break;
            case 4:
                now_serifu = 83;
                break;
            }
            bunkasai_girl7_srf_bank_set();
        }
        break;
    case 9:
        serifu_run();
        break;
    case 10:
        v0 = get_g_zyotai_low(7);
        if (v0 == 4) {
            goto_sub_sub_step(14);
        } else {
            if ((u32)bukatu_pt4 < 100) {
                now_serifu = now_serifu + 1;
                if (get_g_zyotai_low(7) == 2 ||
                    get_g_zyotai_low(7) == 3) {
                    bustup_face_change(4);
                }
            }
            sub_sub_step_up();
        }
        break;
    case 11:
        serifu_run();
        if ((u32)bukatu_pt4 < 100) {
            if ((get_g_zyotai_low(7) == 2 ||
                 get_g_zyotai_low(7) == 3) && now_serifu_in == 5) {
                bustup_face_change(get_g_zyotai_h(7) & 0xFF);
            }
        }
        break;
    case 12:
        v0 = get_g_zyotai_low(7);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = 1;
            break;
        case 2:
            now_serifu = 2;
            break;
        case 3:
            now_serifu = 3;
            break;
        }
        sub_sub_step_up();
        break;
    case 13:
        serifu_run();
        break;
    case 14:
        fade_down_main();
        break;
    case 15:
        fade_out_step();
        break;
    case 16:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 17:
        serifu_run();
        break;
    case 18:
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
