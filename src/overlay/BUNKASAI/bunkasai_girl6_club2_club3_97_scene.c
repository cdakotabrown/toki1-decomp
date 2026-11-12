/* @SymbolStart: 0x8014E9D0 */
/* @RodataBase: 0x8015ED84 */
/* Address: 0x8014E9D8 */
/* Size: 1356 bytes (339 insns) */
/* @SymbolSize: 1364 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=89:v1 */
/* @PostProc: patch_insn_reg_field 0x2b0:rt:at:v1 0x2b8:rs:t6:v1 0x2b8:rt:at:t6 */
#include "types.h"

extern u8 now_sub_sub_step;
extern void sub_sub_step_up(void);
extern void goto_step(s32 a0);
extern void goto_sub_sub_step(s32 a0);
extern s32  get_CD_State2(void);
extern void cd_read_start2(s32 a0, s32 a1, s32 a2);
extern void game_reset(void);
extern void indexed_byte_lsb_set(s32 a0, s32 a1);
extern void value_cache_find_or_load(s32 a0, s32 a1);
extern s32  dec_bg_init(s32 a0);
extern void bustup_cd_read(s32 a0);
extern void bustup_ptr_init(void);
extern void fade_up_main(void);
extern void fade_down_main(void);
extern void fade_in_step(void);
extern void fade_in(void);
extern void fade_out_step(void);
extern void fade_out(void);
extern void bustup_face_change(s32 a0);
extern void zyotai_sprite_set_chk(void);
extern void serifu_run(void);
extern s32  get_g_zyotai_low(s32 a0);
extern void bustup_load_step(s32 a0);
extern void bunkasai_basyo_init(void);
extern void bunkasai_girl6_srf_bank_set(void);
extern void bunkasai_sub_reset(void);

extern s16  now_serifu;
extern u8   now_year;
extern u8   now_girl;
extern u8   now_sub_step;
extern u16  basyo_pack;
extern s32  bunka_sd_kind;
extern s32  bunka_ret_sub_step;
extern s32  bunka_ret_sub_sub_step;
extern s16  mes_base_y;

void bunkasai_girl6_club2_club3_97_scene(void) {
    s32 v0;
    s32 v1;
    s32 state;
    switch (now_sub_sub_step) {
    case 0:
        if (((u32)basyo_pack >> 12) == 2) {
            value_cache_find_or_load(16636, 1);
        } else {
            value_cache_find_or_load(16645, 1);
        }
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
            bustup_cd_read(25133);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in_step();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            bustup_ptr_init();
            now_girl = 6;
            zyotai_sprite_set_chk();
            now_serifu = 41;
        }
        break;
    case 5:
        serifu_run();
        break;
    case 6:
        cd_read_start2(15, 0x801E8000, 19219);
        sub_sub_step_up();
        break;
    case 7:
        fade_up_main();
        break;
    case 8:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(6);
            switch (v0) {
            case 0:
            case 1:
                if (now_year == 96) {
                    now_serifu = 33;
                    break;
                }
                now_serifu = 29;
                break;
            case 2:
                now_serifu = 30;
                break;
            case 3:
                now_serifu = 31;
                break;
            case 4:
                now_serifu = 32;
                break;
            }
            bunkasai_girl6_srf_bank_set();
        }
        break;
    case 9:
        serifu_run();
        break;
    case 10:
        fade_out();
        break;
    case 11:
        bunka_sd_kind = 0;
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_step(128);
        break;
    case 12:
        bustup_load_step(25133);
        break;
    case 13:
        bunkasai_basyo_init();
        mes_base_y = 192;
        bustup_ptr_init();
        now_girl = 6;
        zyotai_sprite_set_chk();
        if (((u32)basyo_pack >> 12) == 3) {
            bustup_face_change(3);
        }
        cd_read_start2(15, 0x801E8000, 19219);
        break;
    case 14:
        v0 = get_g_zyotai_low(6);
        if (v0 == 4) {
            fade_in_step();
        } else {
            fade_in();
        }
        break;
    case 15:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(6);
            switch (v0) {
            case 0: case 1:
                v1 = 3;
                state = (u32)basyo_pack >> 12;
                if (state == v1) {
                    now_serifu = 43;
                } else {
                    now_serifu = (u32)now_year * v1 - 251;
                }
                break;
            case 2:
                v1 = 3;
                state = (u32)basyo_pack >> 12;
                if (state == v1) {
                    now_serifu = 44;
                } else {
                    now_serifu = (u32)now_year * v1 - 250;
                }
                break;
            case 3:
                v1 = 3;
                state = (u32)basyo_pack >> 12;
                if (state == v1) {
                    now_serifu = 45;
                } else {
                    now_serifu = (u32)now_year * v1 - 249;
                }
                break;
            case 4:
                now_serifu = 46;
                break;

                break;
            }
            bunkasai_girl6_srf_bank_set();
        }
        break;
    case 16:
        serifu_run();
        break;
    case 17:
        sub_sub_step_up();
        v0 = get_g_zyotai_low(6);
        switch (v0) {
        case 0: case 1:
            now_serifu = 1;
            return;
        case 2:
            now_serifu = 2;
            return;
        case 3:
            now_serifu = 3;
            return;

        case 4:
            goto_sub_sub_step(19);
            break;
        }
        break;
    case 18:
        serifu_run();
        break;
    case 19:
        v0 = get_g_zyotai_low(6);
        if (v0 == 4) {
            sub_sub_step_up();
        } else {
            fade_down_main();
        }
        break;
    case 20:
        fade_out_step();
        break;
    case 21:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 22:
        serifu_run();
        break;
    case 23:
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
