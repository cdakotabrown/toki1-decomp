/* @SymbolStart: 0x80133990 */
/* @RodataBase: 0x8015BB88 */
/* Address: 0x80133998 */
/* Size: 1152 bytes (288 insns) */
/* @SymbolSize: 1160 */
#include "types.h"

extern void sub_step_up(void);
extern void sub_sub_step_up(void);
extern void goto_sub_sub_step(s32);
extern void device_cmd_send(s32);
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
extern void bunkasai_girl2_vst_bank_init(void);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init(void);

extern s16 now_serifu;
extern u8  now_sub_step;
extern u8  now_sub_sub_step;
extern u8  now_girl;
extern s32 bunka_aisyou;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;

void bunkasai_girl2_vst_bk3_95_fort_main(void) {
    s32 idx = now_sub_sub_step;

    s32 v0;

    switch (idx) {
    case 0:
        value_cache_find_or_load(16645, 1);
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
            bustup_cd_read(21038);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in_step();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            now_serifu = 42;
            bustup_ptr_init();
            now_girl = 2;
            zyotai_sprite_set_chk();
        }
        break;
    case 5:
        serifu_run();
        break;
    case 6:
        cd_read_start2(26, 0x801E8000, 18962);
        sub_sub_step_up();
        break;
    case 7:
        fade_up_main();
        break;
    case 8:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(2);
            switch (v0) {

                break;
            case 0: case 1:
                now_serifu = 104;
                break;
            case 2:
                now_serifu = 105;
                break;
            case 3:
                now_serifu = 106;
                break;
            case 4:
                now_serifu = 108;
                break;
            }
            bunkasai_girl2_vst_bank_init();
        }
        break;
    case 9:
        serifu_run();
        break;
    case 10:
        fade_down_main();
        break;
    case 11:
        v0 = get_g_zyotai_low(2);
        if ((u32)v0 < 3) {
            sub_sub_step_up();
        } else {
            serifu_run();
        }
        break;
    case 12:
        device_cmd_send(514);
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        sub_step_up();
        break;
    case 13:
        cd_read_start2(26, 0x801E8000, 18962);
        sub_sub_step_up();
        break;
    case 14:
        v0 = get_g_zyotai_low(2);
        if ((u32)v0 >= 3) {
            sub_sub_step_up();
        } else {
            fade_up_main();
        }
        break;
    case 15:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(2);
            switch (v0) {

                break;
            case 0: case 1:
                now_serifu = bunka_aisyou * 2 + 110;
                break;
            case 2:
                now_serifu = bunka_aisyou * 2 + 116;
                break;
            case 3: case 4:
                now_serifu = bunka_aisyou + 122;
                break;

                break;
            }
            bunkasai_girl2_vst_bank_init();
        }
        break;
    case 16:
        serifu_run();
        break;
    case 17:
        v0 = get_g_zyotai_low(2);
        if ((u32)v0 < 2 && bunka_aisyou != 1) {
            device_cmd_send(1288);
        }
        sub_sub_step_up();
        break;
    case 18:
        v0 = get_g_zyotai_low(2);
        if ((u32)v0 < 3) {
            serifu_run();
        } else {
            goto_sub_sub_step(23);
        }
        break;
    case 19:
        v0 = get_g_zyotai_low(2);
        if ((u32)v0 < 2) {
            now_serifu = 125;
        } else {
            now_serifu = 127;
        }
        sub_sub_step_up();
        break;
    case 20:
        serifu_run();
        break;
    case 21:
        fade_down_main();
        break;
    case 22:
        serifu_run();
        break;
    case 23:
        fade_out_step();
        break;
    case 24:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 25:
        serifu_run();
        break;
    case 26:
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
