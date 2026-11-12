/* @SymbolStart: 0x80157860 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=66:v1 */
/* @PostProc: patch_insn_reg_field 0xf4:rt:t0:v1 0x100:rt:t0:v1 0x150:rt:t1:t0 0x154:rs:t1:t0 0x16c:rd:t2:t1 0x174:rt:t2:t1 0x178:rt:t2:t1 0x180:rs:t2:t1 0x188:rt:t3:v1 0x194:rt:t3:v1 0x198:rt:t4:v1 0x1a4:rt:t4:v1 0x1a8:rt:t5:v1 0x1b4:rt:t5:v1 0x1b8:rt:t6:v1 0x1c0:rt:t6:v1 0x1f4:rd:t7:t2 0x1fc:rt:t7:t2 0x200:rt:t7:t2 0x208:rs:t7:t2 0x210:rt:t8:v1 0x21c:rt:t8:v1 0x220:rt:t9:v1 0x22c:rt:t9:v1 0x230:rt:t0:v1 0x23c:rt:t0:v1 0x240:rt:t1:v1 0x248:rt:t1:v1 0x27c:rt:t2:t3 0x290:rs:t2:t3 0x290:rt:t2:t3 0x294:rt:t3:v1 0x298:rs:t2:t3 0x2a4:rs:t3:v1 0x2a4:rt:t3:v1 0x2ac:rs:t3:v1 0x2ac:rt:t4:t5 0x2d4:rt:t5:t6 0x2d8:rs:t5:t6 0x2d8:rt:t5:t6 0x2e0:rs:t5:t6 0x2e0:rt:t6:t7 0x2e4:rs:t6:t7 0x308:rt:t8:v1 0x314:rt:t9:v1 0x320:rt:t7:v1 0x32c:rt:t7:v1 0x338:rt:t8:v1 0x344:rt:t9:v1 0x364:rt:t1:v1 0x370:rt:t2:v1 0x37c:rt:t0:v1 0x388:rt:t0:v1 0x394:rt:t1:v1 0x39c:rt:t2:v1 0x3c0:rt:t3:t8 0x3c4:rs:t3:t8 0x3c4:rt:t3:t8 0x3cc:rs:t3:t8 0x3cc:rt:t4:t9 0x3d0:rs:t4:t9 0x3e8:rt:t6:v1 0x3ec:rt:t5:v1 0x3f8:rt:t5:v1 0x400:rt:t6:v1 0x2b0:rt:t4:t5 0x420:rt:t7:t0 0x460:rs:t7:t0 0x460:rt:t7:t0 0x468:rd:t8:t2 0x468:rt:t7:t0 0x46c:rs:t8:t2 0x470:rt:t1:t5 0x478:rd:t9:t3 0x47c:rt:t9:t3 0x480:rs:t1:t5 0x480:rt:t1:t5 0x48c:rd:t2:t6 0x48c:rt:t1:t5 0x490:rs:t2:t6 0x494:rt:t4:t9 0x498:rt:t3:t8 0x4a0:rt:t3:t8 0x4a4:rs:t4:t9 0x4a4:rt:t4:t0 0x4ac:rs:t4:t0 0x4ac:rt:t5:t1 0x4b4:rt:t5:t1 0x4f0:rt:t6:t2 0x4f4:rs:t6:t2 0x4f4:rt:t6:t2 0x4fc:rs:t6:t2 0x4fc:rt:t7:t3 0x500:rs:t7:t3 */
/* @RodataBase: 0x8015FD60 */
/* @SymbolSize: 1396 */
/* Address: 0x80157868, Size: 1388 bytes (347 insns). */
#include "types.h"

extern u8 now_sub_sub_step;
extern u8 now_girl;
extern s16 now_serifu;
extern u16 bukatu_pt4;
extern s16 girl1_syoushin;
extern union Flags800EE960 {
    s32 w;
    u8 b;
} girl1_flag;
extern u8 girl1_basyo;

extern void sub_sub_step_up();
extern void goto_sub_sub_step();
extern s32 get_CD_State2();
extern void cd_read_start2();
extern void game_reset();
extern void indexed_byte_lsb_set();
extern void value_cache_find_or_load();
extern s32 dec_bg_init();
extern void bustup_cd_read();
extern void bustup_ptr_init();
extern void fade_up_main();
extern void fade_down_main();
extern void fade_in_step();
extern void fade_out_step();
extern void zyotai_sprite_set_chk();
extern void serifu_run();
extern s32 get_g_zyotai_low();
extern void bunkasai_sub_reset();
extern void bunkasai_basyo_init();
extern void bunkasai_girl1_clubs3_5_srf_bank_set();

void bunkasai_girl1_club4_95_scene(void)
{
    s16 v1;

    switch ((u32)now_sub_sub_step) {
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
            indexed_byte_lsb_set(1, dec_bg_init(1));
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
            now_serifu = 43;
        }
        break;
    case 5:
        serifu_run();
        break;
    case 6:
        cd_read_start2(18, 0x801E8000, 19095);
        sub_sub_step_up();
        break;
    case 7:
        fade_up_main();
        break;
    case 8:
        if (get_CD_State2() & 1) {
            switch ((u32)get_g_zyotai_low(1)) {
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
            bunkasai_girl1_clubs3_5_srf_bank_set();
        }
        break;
    case 9:
        serifu_run();
        break;
    case 10:
        switch ((u32)get_g_zyotai_low(1)) {
        case 0:
        case 1:
            now_serifu = 77;
            break;
        case 2:
            now_serifu = 80;
            break;
        case 3:
            now_serifu = 83;
            break;
        case 4:
            now_serifu = 86;
            break;
        }
        sub_sub_step_up();
        break;
    case 11:
        serifu_run();
        break;
    case 12:
        if (get_g_zyotai_low(1) == 4) {
            goto_sub_sub_step(18);
        } else {
            if (bukatu_pt4 < 100U) {
                now_serifu = now_serifu + 1;
            }
            sub_sub_step_up();
        }
        break;
    case 13:
        serifu_run();
        break;
    case 14:
        if ((girl1_syoushin & 3) == 0) {
            switch ((u32)get_g_zyotai_low(1)) {
            case 0:
            case 1:
                now_serifu = 103;
                break;
            case 2:
                now_serifu = 104;
                break;
            case 3:
                now_serifu = 105;
                break;
            }
        } else {
            switch ((u32)get_g_zyotai_low(1)) {
            case 0:
            case 1:
                now_serifu = 88;
                break;
            case 2:
                now_serifu = 89;
                break;
            case 3:
                now_serifu = 90;
                break;
            }
        }
        sub_sub_step_up();
        break;
    case 15:
        serifu_run();
        break;
    case 16:
        if ((girl1_syoushin & 3) == 0) {
            if ((u32)get_g_zyotai_low(1) < 2U) {
                now_serifu = 13;
            } else {
                now_serifu = 12;
            }
            goto_sub_sub_step(18);
        } else {
            switch ((u32)get_g_zyotai_low(1)) {
            case 0:
            case 1:
                v1 = 91;
                break;
            case 2:
                v1 = 92;
                break;
            case 3:
                v1 = 93;
                break;
            default:
                v1 = now_serifu;
                break;
            }
            if ((girl1_flag.w << 29) >= 0) {
                v1 = v1 + 3;
            }
            now_serifu = v1;
            if (((u32)girl1_basyo >> 4) == 1) {
                now_serifu = v1 + 6;
            }
            girl1_flag.b = girl1_flag.b | 4;
            sub_sub_step_up();
        }
        break;
    case 17:
        serifu_run();
        break;
    case 18:
        fade_down_main();
        break;
    case 19:
        if (get_g_zyotai_low(1) == 4 || (girl1_syoushin & 3) == 0) {
            serifu_run();
        } else {
            sub_sub_step_up();
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
