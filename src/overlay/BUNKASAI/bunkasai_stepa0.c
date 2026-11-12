/* @SymbolStart: 0x801417DC */
/* @RodataBase: 0x8015D520 */
/* Address: 0x801417E4 */
/* Size: 1932 bytes (483 insns) */
/* @SymbolSize: 1940 */
/* @CompileEnv: IDO52_CONST_INREG_VALUES=13,10 */
/* @PostProc: swap_instructions --offset1 0x4a4 --offset2 0x4b8 */
/* @PostProc: swap_instructions --offset1 0x4b8 --offset2 0x4bc */
/* @PostProc: swap_instructions --offset1 0x4bc --offset2 0x4c0 */
/* @PostProc: swap_instructions --offset1 0x5c8 --offset2 0x5dc */
/* @PostProc: swap_instructions --offset1 0x5dc --offset2 0x5e0 */
/* @PostProc: swap_instructions --offset1 0x5e0 --offset2 0x5e4 */
#include "types.h"

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
extern void date_name_set(u8);
extern void serifu_run(void);
extern s32 get_g_zyotai_low(s32);
extern void bustup_load_step(s32);
extern void memcpy(s32, s32, s32);
extern void bunkasai_girl10_srf_bank_set(void);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init(void);

extern s16 now_serifu;
extern u8  now_girl;
extern s32 kuchi_stop;

extern u8 now_sub_sub_step;
void bunkasai_stepa0(void) {
    s32 idx = now_sub_sub_step;

    s32 v0;
    s32 gid;

    switch (idx) {
    case 0:
        value_cache_find_or_load(16664, 1);
        sub_sub_step_up();
        break;
    case 1:
        fade_out_step();
        break;
    case 2:
        if (get_CD_State2() & 1) {
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            sub_sub_step_up();
        }
        break;
    case 3:
        bustup_load_step(27743);
        break;
    case 4:
        v0 = get_g_zyotai_low(10);
        if (v0 == 4) {
            bunkasai_basyo_init();
            cd_read_start2(19, 0x801E8000, 19294);
            goto_sub_sub_step(50);
            break;
        }


        bunkasai_basyo_init();
        memcpy(0x80197000, 0x80180000, 0x16800);
        bustup_cd_read(26933);
        sub_sub_step_up();
        break;
    case 5:
        fade_in_step();
        break;
    case 6:
        if (get_CD_State2() & 1) {
            memcpy(0x801AE000, 0x80180000, 0x16800);
            bustup_ptr_init();
            now_girl = 10;
            zyotai_sprite_set_chk();
            now_serifu = 44;
        }
        break;
    case 7:
        serifu_run();
        break;
    case 8:
        cd_read_start2(19, 0x801E8000, 19294);
        sub_sub_step_up();
        break;
    case 9:
        fade_up_main();
        break;
    case 10:
        if (get_CD_State2() & 1) {
            bunkasai_girl10_srf_bank_set();
            v0 = get_g_zyotai_low(10);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 67;
                break;
            case 2:
                now_serifu = 81;
                break;
            case 3:
                now_serifu = 93;
                break;
            }
        }
        break;
    case 11:
        serifu_run();
        break;
    case 12:
        fade_down_main();
        break;
    case 13:
        memcpy(0x80180000, 0x80197000, 0x16800);
        bustup_ptr_init();
        gid = 13;
        now_girl = gid;
        date_name_set(gid);
        zyotai_sprite_set_chk();
        break;
    case 14:
        fade_up_main();
        break;
    case 15:
        serifu_run();
        break;
    case 16:
        fade_down_main();
        break;
    case 17:
        memcpy(0x80180000, 0x801AE000, 0x16800);
        bustup_ptr_init();
        gid = 10;
        now_girl = gid;
        date_name_set(gid);
        zyotai_sprite_set_chk();
        break;
    case 18:
        fade_up_main();
        break;
    case 19:
        serifu_run();
        break;
    case 20:
        fade_down_main();
        break;
    case 21:
        memcpy(0x80180000, 0x80197000, 0x16800);
        bustup_ptr_init();
        gid = 13;
        now_girl = gid;
        date_name_set(gid);
        zyotai_sprite_set_chk();
        break;
    case 22:
        fade_up_main();
        break;
    case 23:
        serifu_run();
        break;
    case 24:
        kuchi_stop = 1;
        gid = 10;
        now_girl = gid;
        date_name_set(gid);
        sub_sub_step_up();
        break;
    case 25:
        serifu_run();
        break;
    case 26:
        kuchi_stop = 0;
        gid = 13;
        now_girl = gid;
        date_name_set(gid);
        sub_sub_step_up();
        break;
    case 27:
        serifu_run();
        break;
    case 28:
        kuchi_stop = 1;
        gid = 10;
        now_girl = gid;
        date_name_set(gid);
        sub_sub_step_up();
        break;
    case 29:
        serifu_run();
        break;
    case 30:
        kuchi_stop = 0;
        gid = 13;
        now_girl = gid;
        date_name_set(gid);
        sub_sub_step_up();
        break;
    case 31:
        serifu_run();
        break;
    case 32:
        v0 = get_g_zyotai_low(10);
        if (v0 == 3) {
            goto_sub_sub_step(34);
        } else {
            kuchi_stop = 1;
            gid = 10;
        now_girl = gid;
        date_name_set(gid);
            sub_sub_step_up();
        }
        break;
    case 33:
        serifu_run();
        break;
    case 34:
        v0 = get_g_zyotai_low(10);
        if (v0 == 3) {
            goto_sub_sub_step(36);
        } else {
            kuchi_stop = 0;
            gid = 13;
        now_girl = gid;
        date_name_set(gid);
            sub_sub_step_up();
        }
        break;
    case 35:
        serifu_run();
        break;
    case 36:
        fade_down_main();
        break;
    case 37:
        memcpy(0x80180000, 0x801AE000, 0x16800);
        bustup_ptr_init();
        gid = 10;
        now_girl = gid;
        date_name_set(gid);
        zyotai_sprite_set_chk();
        break;
    case 38:
        fade_up_main();
        break;
    case 39:
        serifu_run();
        break;
    case 40:
        v0 = get_g_zyotai_low(10);
        if ((u32)v0 >= 2) {
            goto_sub_sub_step(42);
        } else {
            kuchi_stop = 1;
            gid = 13;
        now_girl = gid;
        date_name_set(gid);
            sub_sub_step_up();
        }
        break;
    case 41:
        serifu_run();
        break;
    case 42:
        kuchi_stop = 0;
        gid = 10;
        now_girl = gid;
        date_name_set(gid);
        sub_sub_step_up();
        break;
    case 43:
        if (get_g_zyotai_low(10) == 3 || get_g_zyotai_low(10) == 2) {
            sub_sub_step_up();
        } else {
            serifu_run();
        }
        break;
    case 44:
        fade_down_main();
        break;
    case 45:
        serifu_run();
        break;
    case 46:
        fade_out_step();
        break;
    case 47:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 48:
        serifu_run();
        break;
    case 49:
        bunkasai_sub_reset();
        break;
    case 50:
        fade_in_step();
        break;
    case 51:
        if (get_CD_State2() & 1) {
            bunkasai_girl10_srf_bank_set();
            now_serifu = 103;
            bustup_ptr_init();
            gid = 13;
        now_girl = gid;
        date_name_set(gid);
            zyotai_sprite_set_chk();
        }
        break;
    case 52: case 53:
        fade_up_main();
        break;
    case 54:
        serifu_run();
        break;
    case 55:
        fade_down_main();
        break;
    case 56:
        date_name_set(10);
        goto_sub_sub_step(45);
        break;
    default:
        game_reset();
        break;
    }
}
