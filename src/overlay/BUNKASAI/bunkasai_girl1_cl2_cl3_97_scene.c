/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:169 */
/* @SymbolStart: 0x80156284 */
/* @SymbolSize: 1848 */
/* @RodataBase: 0x8015FAD0 */
/* @PostProc: patch_insn_reg_field 0x11c:rt:t2:v1 0x128:rt:t2:v1 0x178:rt:t3:t2 0x17c:rs:t3:t2 0x194:rd:t4:t3 0x19c:rt:t4:t3 0x1a0:rt:t4:t3 0x1a8:rs:t4:t3 0x1b0:rt:t5:v1 0x1bc:rt:t5:v1 0x1c0:rt:t6:v1 0x1cc:rt:t6:v1 0x1d0:rt:t7:v1 0x1dc:rt:t7:v1 0x1e0:rt:t8:v1 0x1e8:rt:t8:v1 0x214:rt:t9:t4 0x21c:rd:t0:t6 0x21c:rt:t9:t4 0x220:rs:t0:t6 0x238:rt:t1:t7 0x23c:rs:t1:t7 0x23c:rt:t1:t7 0x240:rt:t4:v1 0x244:rd:t2:t8 0x244:rt:t1:t7 0x248:rs:t2:t8 0x250:rt:t3:v1 0x25c:rt:t3:v1 0x260:rt:t4:v1 0x2b0:rt:t6:v1 0x2bc:rt:t7:v1 0x2c8:rt:t5:v1 0x2d4:rt:t5:v1 0x2e0:rt:t6:v1 0x2e8:rt:t7:v1 0x31c:rd:t8:t9 0x324:rt:t8:t9 0x328:rt:t8:t9 0x330:rs:t8:t9 0x338:rt:t9:v1 0x344:rt:t9:v1 0x348:rt:t0:v1 0x354:rt:t0:v1 0x358:rt:t1:v1 0x364:rt:t1:v1 0x368:rt:t2:v1 0x370:rt:t2:v1 0x3a4:rt:t3:t0 0x3a8:rs:t3:t0 0x3a8:rt:t3:t0 0x3b0:rd:t4:t1 0x3b0:rt:t3:t0 0x3b4:rs:t4:t1 0x3b8:rt:t5:t2 0x3d0:rs:t5:t2 0x3d0:rt:t5:t2 0x3d8:rt:t5:t2 0x3e0:rt:t6:t3 0x3e4:rt:t6:t3 0x410:rt:t7:t4 0x41c:rt:t7:t4 0x420:rt:t8:t5 0x42c:rt:t8:t5 0x464:rt:t9:t6 0x468:rs:t9:t6 0x480:rd:t0:t7 0x488:rt:t0:t7 0x48c:rt:t0:t7 0x494:rs:t0:t7 0x49c:rt:t1:v1 0x4a8:rt:t1:v1 0x4ac:rt:t2:v1 0x4b8:rt:t2:v1 0x4bc:rt:t3:v1 0x4c8:rt:t3:v1 0x4cc:rt:t4:v1 0x4d4:rt:t4:v1 0x53c:rt:t5:t8 0x540:rs:t5:t8 0x540:rt:t5:t8 0x548:rs:t5:t8 0x548:rt:t6:t9 0x54c:rs:t6:t9 0x570:rt:t8:v1 0x57c:rt:t9:v1 0x588:rt:t7:v1 0x594:rt:t7:v1 0x5a0:rt:t8:v1 0x5ac:rt:t9:v1 0x608:rd:t1:t2 0x60c:rs:t1:t2 0x618:rd:t2:t3 0x61c:rt:t2:t3 0x624:rt:t4:t5 0x62c:rd:t5:t6 0x62c:rt:t4:t5 0x630:rs:t5:t6 0x634:rt:t6:t8 0x63c:rt:t6:t8 0x640:rt:t7:t0 0x648:rs:t7:t0 0x648:rt:t8:t1 0x64c:rt:t8:t1 0x69c:rt:t0:v1 0x6b4:rt:t9:v1 0x6c0:rt:t9:v1 0x6c8:rt:t0:v1 */
/* Address: 0x8015628C */
/* Size: 1840 bytes (460 insns) */
/* @SymbolSize: 1848 (head absorbed) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void goto_step(s32);
extern void goto_sub_sub_step(s32);
extern s32  get_CD_State2(void);
extern void cd_read_start2(s32, s32, s32);
extern void game_reset(void);
extern void indexed_byte_lsb_set(s32, s32);
extern void value_cache_find_or_load(s32, s32);
extern s32  dec_bg_init(s32);
extern void bustup_cd_read(s32);
extern void bustup_ptr_init(void);
extern void fade_up_main(void);
extern void fade_down_main(void);
extern void fade_in_step(void);
extern void fade_out_step(void);
extern void fade_out(void);
extern void zyotai_sprite_set_chk(void);
extern void serifu_run(void);
extern s32  get_g_zyotai_low(s32);
extern void bustup_load_step(s32);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init(void);
extern void bunkasai_girl1_clubs0_2_srf_bank_set(void);

extern s16 now_serifu;
extern volatile u8 screen_w[];
extern u16 basyo_pack;
extern s16 girl1_syoushin;
extern u8  girl1_basyo;
extern u8  now_girl;
extern u8  now_sub_step;
extern u8  now_sub_sub_step;
extern s32 bunka_sd_kind;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 mes_base_y;

void bunkasai_girl1_cl2_cl3_97_scene(void) {
    s32 idx = now_sub_sub_step;

    s32 v0;
    s32 v1;

    switch (idx) {
    case 0:
        if (((u32) basyo_pack >> 12) == 2) {
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
            bustup_cd_read(23873);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in_step();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            bustup_ptr_init();
            now_girl = 1;
            zyotai_sprite_set_chk();
            v1 = 41;
            now_serifu = v1;
        }
        break;
    case 5:
        serifu_run();
        break;
    case 6:
        cd_read_start2(20, 0x801E8000, 19075);
        sub_sub_step_up();
        break;
    case 7:
        fade_up_main();
        break;
    case 8:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(1);
            switch (v0) {
            case 0:
            case 1:
                v1 = 1;
                now_serifu = v1;
                break;
            case 2:
                v1 = 2;
                now_serifu = v1;
                break;
            case 3:
                v1 = 3;
                now_serifu = v1;
                break;
            case 4:
                v1 = 5;
                now_serifu = v1;
                break;
            }
            bunkasai_girl1_clubs0_2_srf_bank_set();
        }
        break;
    case 9:
        serifu_run();
        break;
    case 10:
        if ((*(volatile s32 *) &screen_w[512] << 29) < 0
            && (u32) get_g_zyotai_low(1) < 4U) {
            if (((u32) girl1_basyo >> 4) == 0) {
                v1 = 10;
                now_serifu = v1;
            } else {
                v1 = 11;
                now_serifu = v1;
            }
            sub_sub_step_up();
        } else {
            goto_sub_sub_step(14);
        }
        break;
    case 11:
        serifu_run();
        break;
    case 12:
        v0 = get_g_zyotai_low(1);
        switch (v0) {
        case 0:
        case 1:
            v1 = 83;
            now_serifu = v1;
            break;
        case 2:
            v1 = 84;
            now_serifu = v1;
            break;
        case 3:
            v1 = 85;
            now_serifu = v1;
            break;
        }
        sub_sub_step_up();
        break;
    case 13:
        serifu_run();
        break;
    case 14:
        v0 = get_g_zyotai_low(1);
        switch (v0) {
        case 0:
        case 1:
            v1 = 86;
            now_serifu = v1;
            break;
        case 2:
            v1 = 87;
            now_serifu = v1;
            break;
        case 3:
            v1 = 88;
            now_serifu = v1;
            break;
        case 4:
            v1 = 89;
            now_serifu = v1;
            break;
        }
        sub_sub_step_up();
        break;
    case 15:
        serifu_run();
        break;
    case 16:
        fade_out();
        break;
    case 17:
        if (((u32) basyo_pack >> 12) == 2) {
            bunka_sd_kind = 0;
        } else {
            bunka_sd_kind = 0;
        }
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_step(128);
        break;
    case 18:
        bustup_load_step(23873);
        break;
    case 19:
        bunkasai_basyo_init();
        mes_base_y = 192;
        bustup_ptr_init();
        now_girl = 1;
        zyotai_sprite_set_chk();
        cd_read_start2(20, 0x801E8000, 19075);
        break;
    case 20:
        fade_in_step();
        break;
    case 21:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(1);
            switch (v0) {
            case 0:
            case 1:
                v1 = 137;
                now_serifu = v1;
                break;
            case 2:
                v1 = 139;
                now_serifu = v1;
                break;
            case 3:
                v1 = 141;
                now_serifu = v1;
                break;
            case 4:
                v1 = 143;
                now_serifu = v1;
                break;
            }
            bunkasai_girl1_clubs0_2_srf_bank_set();
        }
        break;
    case 22:
        serifu_run();
        break;
    case 23:
        if (get_g_zyotai_low(1) == 4) {
            goto_sub_sub_step(30);
        } else {
            fade_up_main();
        }
        break;
    case 24:
        serifu_run();
        break;
    case 25:
        if ((girl1_syoushin & 3) == 0) {
            v0 = get_g_zyotai_low(1);
            switch (v0) {
            case 0:
            case 1:
                v1 = 156;
                now_serifu = v1;
                break;
            case 2:
                v1 = 157;
                now_serifu = v1;
                break;
            case 3:
                v1 = 158;
                now_serifu = v1;
                break;
            }
        } else {
            v0 = get_g_zyotai_low(1);
            switch (v0) {
            case 0:
            case 1:
                v1 = 144;
                break;
            case 2:
                v1 = 145;
                break;
            case 3:
                v1 = 146;
                break;
            default:
                v1 = now_serifu;
                break;
            }
            if ((*(volatile s32 *) &screen_w[512] << 29) >= 0) {
                v1 = (s16) (v1 + 3);
            }
            now_serifu = v1;
            if (((u32) screen_w[514] >> 4) == 1) {
                now_serifu = v1 + 6;
            }
            screen_w[512] = screen_w[512] | 4;
        }
        sub_sub_step_up();
        break;
    case 26:
        serifu_run();
        break;
    case 27:
        fade_down_main();
        break;
    case 28:
        v0 = get_g_zyotai_low(1);
        switch (v0) {
        case 0:
        case 1:
            v1 = 13;
            now_serifu = v1;
            break;
        case 2:
        case 3:
            v1 = 12;
            now_serifu = v1;
            break;
        }
        sub_sub_step_up();
        break;
    case 29:
        serifu_run();
        break;
    case 30:
        fade_out_step();
        break;
    case 31:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 32:
        serifu_run();
        break;
    case 33:
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
