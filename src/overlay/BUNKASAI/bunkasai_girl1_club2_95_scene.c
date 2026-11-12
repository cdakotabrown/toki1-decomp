/* @SymbolStart: 0x80155388 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=100:a0 IDO52_COPYPROP_SITE=lod:6:deny,lod:19:deny,lod:20:deny IDO52_COPYPROP_VAR=-12 */
/* @RodataBase: 0x8015F948 */
/* Address: 0x80155390 */
/* Size: 1932 bytes (483 insns) */
/* @SymbolSize: 1940 */
/* @PostProc: patch_insn_reg_field 0xf4:rt:t0:a0 0x100:rt:t0:a0 0x150:rt:t1:t0 0x154:rs:t1:t0 0x16c:rd:t2:t1 0x174:rt:t2:t1 0x178:rt:t2:t1 0x180:rs:t2:t1 0x188:rt:t3:a0 0x194:rt:t3:a0 0x198:rt:t4:a0 0x1a4:rt:t4:a0 0x1a8:rt:t5:a0 0x1b4:rt:t5:a0 0x1b8:rt:t6:a0 0x1c0:rt:t6:a0 0x1ec:rt:t7:t2 0x1f4:rd:t8:t4 0x1f4:rt:t7:t2 0x1f8:rs:t8:t4 0x210:rt:t9:t5 0x214:rs:t9:t5 0x214:rt:t9:t5 0x218:rt:t2:a0 0x21c:rd:t0:t6 0x21c:rt:t9:t5 0x220:rs:t0:t6 0x228:rt:t1:a0 0x234:rt:t1:a0 0x238:rt:t2:a0 0x288:rt:t4:a0 0x294:rt:t5:a0 0x2a0:rt:t3:a0 0x2ac:rt:t3:a0 0x2b8:rt:t4:a0 0x2c0:rt:t5:a0 0x2f4:rd:t6:t7 0x2fc:rt:t6:t7 0x300:rt:t6:t7 0x308:rs:t6:t7 0x310:rt:t7:a0 0x31c:rt:t7:a0 0x320:rt:t8:a0 0x32c:rt:t8:a0 0x330:rt:t9:a0 0x33c:rt:t9:a0 0x340:rt:t0:a0 0x348:rt:t0:a0 0x380:rt:t1:t8 0x384:rs:t1:t8 0x384:rt:t1:t8 0x390:rt:t1:t8 0x398:rt:t2:t9 0x39c:rt:t2:t9 0x3c8:rt:t3:t0 0x3d4:rt:t3:t0 0x3d8:rt:t4:t1 0x3e4:rt:t4:t1 0x41c:rt:t5:t2 0x420:rs:t5:t2 0x438:rd:t6:t3 0x440:rt:t6:t3 0x444:rt:t6:t3 0x44c:rs:t6:t3 0x454:rt:t7:a0 0x460:rt:t7:a0 0x464:rt:t8:a0 0x470:rt:t8:a0 0x474:rt:t9:a0 0x480:rt:t9:a0 0x484:rt:t0:a0 0x48c:rt:t0:a0 0x510:rt:t2:a0 0x51c:rt:t3:a0 0x528:rt:t1:a0 0x534:rt:t1:a0 0x540:rt:t2:a0 0x548:rt:t3:a0 0x5a0:rt:t7:a0 0x5ac:rt:t8:a0 0x5b8:rt:t6:a0 0x5c4:rt:t6:a0 0x5d0:rt:t7:a0 0x5dc:rt:t8:a0 0x630:rt:t9:t6 0x638:rd:t0:t8 0x638:rt:t9:t6 0x63c:rs:t0:t8 0x648:rd:t1:t9 0x64c:rt:t1:t9 0x654:rt:t3:t1 0x65c:rd:t4:t2 0x65c:rt:t3:t1 0x660:rs:t4:t2 0x664:rt:t5:t4 0x66c:rt:t5:t4 0x6c0:rt:t0:a0 0x6cc:rt:t0:a0 0x6e0:rt:t2:a0 0x6e4:rt:t1:a0 0x6f0:rt:t1:a0 0x6fc:rt:t2:a0 */
#include "types.h"

extern u8 now_sub_sub_step;
extern u8 screen_w[];

extern s16 now_serifu;
extern u8  girl1_basyo;
extern s16 girl1_syoushin;
extern u8  now_girl;
extern u8  now_sub_step;
extern s32 bunka_sd_kind;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 mes_base_y;

extern s32 sub_sub_step_up(void);
extern s32 goto_step(s32);
extern s32 goto_sub_sub_step(s32);
extern s32 get_CD_State2(void);
extern s32 cd_read_start2(s32, s32, s32);
extern s32 game_reset(void);
extern s32 indexed_byte_lsb_set(s32, s32);
extern s32 value_cache_find_or_load(s32, s32);
extern s32 dec_bg_init(s32);
extern s32 bustup_cd_read(s32);
extern s32 bustup_ptr_init(void);
extern s32 fade_up_main(void);
extern s32 fade_down_main(void);
extern s32 fade_in_step(void);
extern s32 fade_out_step(void);
extern s32 fade_out(void);
extern s32 zyotai_sprite_set_chk(void);
extern s32 serifu_run(void);
extern s32 get_g_zyotai_low(s32);
extern s32 bustup_load_step(s32);
extern s32 bunkasai_sub_reset(void);
extern s32 bunkasai_basyo_init(void);
extern s32 bunkasai_girl1_clubs0_2_srf_bank_set(void);

void bunkasai_girl1_club2_95_scene(void)
{
    s32 sel;
    s32 v0;
    u8 *bp;

    switch ((u32)now_sub_sub_step) {
    case 0:
        value_cache_find_or_load(16636, 1);
        sub_sub_step_up();
        break;
    case 1:
        fade_out_step();
        break;
    case 2:
        v0 = get_CD_State2();
        if (v0 & 1) {
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
        v0 = get_CD_State2();
        if (v0 & 1) {
            bustup_ptr_init();
            now_girl = 1;
            zyotai_sprite_set_chk();
            now_serifu = 41;
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
        v0 = get_CD_State2();
        if (v0 & 1) {
            v0 = get_g_zyotai_low(1);
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
            case 4:
                now_serifu = 5;
                break;
            }
            bunkasai_girl1_clubs0_2_srf_bank_set();
        }
        break;
    case 9:
        serifu_run();
        break;
    case 10:
        bp = screen_w;
        if ((s32)(*(s32 *)(bp + 512) << 29) < 0 && get_g_zyotai_low(1) != 4) {
            if (((u32)girl1_basyo >> 4) == 0) {
                now_serifu = 10;
            } else {
                now_serifu = 11;
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
            now_serifu = 83;
            break;
        case 2:
            now_serifu = 84;
            break;
        case 3:
            now_serifu = 85;
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
            now_serifu = 86;
            break;
        case 2:
            now_serifu = 87;
            break;
        case 3:
            now_serifu = 88;
            break;
        case 4:
            now_serifu = 89;
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
        fade_in_step();
        break;
    case 21:
        v0 = get_CD_State2();
        if (v0 & 1) {
            v0 = get_g_zyotai_low(1);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 90;
                break;
            case 2:
                now_serifu = 92;
                break;
            case 3:
                now_serifu = 94;
                break;
            case 4:
                now_serifu = 96;
                break;
            }
            bunkasai_girl1_clubs0_2_srf_bank_set();
        }
        break;
    case 22:
        serifu_run();
        break;
    case 23:
        v0 = get_g_zyotai_low(1);
        if (v0 == 4) {
            goto_sub_sub_step(30);
        } else {
            fade_up_main();
        }
        break;
    case 24:
        serifu_run();
        break;
    case 25:
        v0 = get_g_zyotai_low(1);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = 97;
            break;
        case 2:
            now_serifu = 98;
            break;
        case 3:
            now_serifu = 99;
            break;
        }
        sub_sub_step_up();
        break;
    case 26:
        serifu_run();
        break;
    case 27:
        if ((girl1_syoushin & 3) == 0) {
            v0 = get_g_zyotai_low(1);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 112;
                break;
            case 2:
                now_serifu = 113;
                break;
            case 3:
                now_serifu = 114;
                break;
            }
        } else {
            v0 = get_g_zyotai_low(1);
            switch (v0) {
            case 0:
            case 1:
                sel = 100;
                break;
            case 2:
                sel = 101;
                break;
            case 3:
                sel = 102;
                break;
            default:
                sel = now_serifu;
                break;
            }
            bp = screen_w;
            if ((s32)(*(s32 *)(bp + 512) << 29) >= 0) {
                sel = (s16)(sel + 3);
            }
            now_serifu = sel;
            if (((u32)bp[514] >> 4) == 1) {
                now_serifu = sel + 6;
            }
            bp[512] = bp[512] | 4;
        }
        sub_sub_step_up();
        break;
    case 28:
        serifu_run();
        break;
    case 29:
        fade_down_main();
        bp = screen_w;
        if (((u32)bp[514] >> 4) == 1) {
            now_serifu = 12;
        } else {
            v0 = get_g_zyotai_low(1);
            if ((u32)v0 < 2U) {
                now_serifu = 13;
            } else {
                now_serifu = 12;
            }
        }
        break;
    case 30:
        v0 = get_g_zyotai_low(1);
        if ((u32)v0 < 4U) {
            serifu_run();
        } else {
            fade_out_step();
        }
        break;
    case 31:
        v0 = get_g_zyotai_low(1);
        if ((u32)v0 < 4U) {
            fade_out_step();
        } else {
            sub_sub_step_up();
        }
        now_serifu = 0;
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
