/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:169 */
/* @SymbolStart: 0x80155B1C */
/* @SymbolSize: 1896 */
/* @RodataBase: 0x8015FA0C */
/* @PostProc: swap_instructions --offset1 0x514 --offset2 0x564 */
/* @PostProc: swap_instructions --offset1 0x564 --offset2 0x568 */
/* @PostProc: patch_insn_reg_field 0xf4:rt:t0:v1 0x100:rt:t0:v1 0x150:rt:t1:t0 0x154:rs:t1:t0 0x16c:rd:t2:t1 0x174:rt:t2:t1 0x178:rt:t2:t1 0x180:rs:t2:t1 0x188:rt:t3:v1 0x194:rt:t3:v1 0x198:rt:t4:v1 0x1a4:rt:t4:v1 0x1a8:rt:t5:v1 0x1b4:rt:t5:v1 0x1b8:rt:t6:v1 0x1c0:rt:t6:v1 0x1ec:rt:t7:t2 0x1f4:rd:t8:t4 0x1f4:rt:t7:t2 0x1f8:rs:t8:t4 0x210:rt:t9:t5 0x214:rs:t9:t5 0x214:rt:t9:t5 0x218:rt:t2:v1 0x21c:rd:t0:t6 0x21c:rt:t9:t5 0x220:rs:t0:t6 0x228:rt:t1:v1 0x234:rt:t1:v1 0x238:rt:t2:v1 0x288:rt:t4:v1 0x294:rt:t5:v1 0x2a0:rt:t3:v1 0x2ac:rt:t3:v1 0x2b8:rt:t4:v1 0x2c0:rt:t5:v1 0x2f4:rd:t6:t7 0x2fc:rt:t6:t7 0x300:rt:t6:t7 0x308:rs:t6:t7 0x310:rt:t7:v1 0x31c:rt:t7:v1 0x320:rt:t8:v1 0x32c:rt:t8:v1 0x330:rt:t9:v1 0x33c:rt:t9:v1 0x340:rt:t0:v1 0x348:rt:t0:v1 0x380:rt:t1:t8 0x384:rs:t1:t8 0x384:rt:t1:t8 0x390:rt:t1:t8 0x398:rt:t2:t9 0x39c:rt:t2:t9 0x3c8:rt:t3:t0 0x3d4:rt:t3:t0 0x3d8:rt:t4:t1 0x3e4:rt:t4:t1 0x440:rt:t5:t2 0x444:rs:t5:t2 0x45c:rd:t6:t3 0x464:rt:t6:t3 0x468:rt:t6:t3 0x470:rs:t6:t3 0x478:rt:t7:v1 0x484:rt:t7:v1 0x488:rt:t8:v1 0x494:rt:t8:v1 0x498:rt:t9:v1 0x4a4:rt:t9:v1 0x4a8:rt:t0:v1 0x4b0:rt:t0:v1 0x528:rt:t2:v1 0x534:rt:t3:v1 0x540:rt:t1:v1 0x54c:rt:t1:v1 0x558:rt:t2:v1 0x560:rt:t3:v1 0x578:rt:t6:t7 0x580:rt:t6:t7 0x5a4:rt:t7:t8 0x5a8:rs:t7:t8 0x5a8:rt:t7:t8 0x5b0:rs:t7:t8 0x5b0:rt:t8:t9 0x5b4:rs:t8:t9 0x5d8:rt:t0:v1 0x5f0:rt:t9:v1 0x5fc:rt:t9:v1 0x604:rt:t0:v1 0x668:rt:t1:t0 0x670:rt:t1:t0 0x69c:rt:t7:t8 0x6a4:rt:t7:t8 0x6a8:rt:t8:t0 0x6b0:rs:t8:t0 0x6b0:rt:t9:t1 0x6b8:rt:t9:t1 0x6e4:rt:t0:t2 0x6e8:rs:t0:t2 0x6e8:rt:t0:t2 0x6f0:rs:t0:t2 0x6f0:rt:t1:t3 0x6f4:rs:t1:t3 */
/* Address: 0x80155B24 */
/* Size: 1888 bytes (472 insns) */
/* @SymbolSize: 1896 (head absorbed) */
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
extern void fade_in(void);
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
extern s16 girl1_syoushin;
extern u8  girl1_basyo;
extern u8  now_girl;
extern u8  now_sub_step;
extern u8  now_sub_sub_step;
extern s32 bunka_sd_kind;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 mes_base_y;

void bunkasai_girl1_club2_96_scene(void) {
    s32 idx = now_sub_sub_step;

    s32 v0;
    s32 v1;

    switch (idx) {
    case 0:
        value_cache_find_or_load(16636, 1);
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
            && get_g_zyotai_low(1) != 4) {
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
        bunka_sd_kind = 0;
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
        if (get_g_zyotai_low(1) == 4) {
            fade_in_step();
        } else {
            fade_in();
        }
        break;
    case 21:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(1);
            switch (v0) {
            case 0:
            case 1:
                v1 = 115;
                now_serifu = v1;
                break;
            case 2:
                v1 = 116;
                now_serifu = v1;
                break;
            case 3:
                v1 = 117;
                now_serifu = v1;
                break;
            case 4:
                v1 = 118;
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
            sub_sub_step_up();
        }
        break;
    case 24:
        v0 = get_g_zyotai_low(1);
        switch (v0) {
        case 0:
        case 1:
            v1 = 119;
            now_serifu = v1;
            break;
        case 2:
            v1 = 120;
            now_serifu = v1;
            break;
        case 3:
            v1 = 121;
            now_serifu = v1;
            break;
        }
        v0 = girl1_syoushin;
        v1 = now_serifu;
        if ((v0 & 3) == 0) {
            now_serifu = v1 + 15;
        }
        sub_sub_step_up();
        break;
    case 25:
        serifu_run();
        break;
    case 26:
        if ((girl1_syoushin & 3) == 0) {
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
            goto_sub_sub_step(28);
        } else {
            v0 = get_g_zyotai_low(1);
            switch (v0) {
            case 0:
            case 1:
                v1 = 122;
                break;
            case 2:
                v1 = 123;
                break;
            case 3:
                v1 = 124;
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
            sub_sub_step_up();
        }
        break;
    case 27:
        serifu_run();
        break;
    case 28:
        fade_down_main();
        break;
    case 29:
        if ((girl1_syoushin & 3) == 0) {
            serifu_run();
        } else {
            sub_sub_step_up();
        }
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
