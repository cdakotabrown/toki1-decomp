/* @SymbolStart: 0x801580EC */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:162 */
/* @RodataBase: 0x8015FE64 */
/* Address: 0x801580F4 */
/* Size: 1648 bytes (412 insns) */
/* @SymbolSize: 1656 */
/* @PostProc: patch_insn_reg_field 0xf4:rt:t0:v1 0x100:rt:t0:v1 0x150:rt:t1:t0 0x154:rs:t1:t0 0x16c:rd:t2:t1 0x174:rt:t2:t1 0x178:rt:t2:t1 0x180:rs:t2:t1 0x188:rt:t3:v1 0x194:rt:t3:v1 0x198:rt:t4:v1 0x1a4:rt:t4:v1 0x1a8:rt:t5:v1 0x1b4:rt:t5:v1 0x1b8:rt:t6:v1 0x1c0:rt:t6:v1 0x1f4:rt:t7:v1 0x200:rt:t7:v1 0x20c:rt:t8:t2 0x210:rt:t0:v1 0x214:rd:t9:t4 0x214:rt:t8:t2 0x218:rs:t9:t4 0x21c:rt:t1:t5 0x228:rt:t0:v1 0x22c:rs:t1:t5 0x22c:rt:t1:t5 0x230:rt:t4:v1 0x234:rd:t2:t6 0x234:rt:t1:t5 0x238:rs:t2:t6 0x240:rt:t3:v1 0x24c:rt:t3:v1 0x250:rt:t4:v1 0x2a0:rt:t5:t7 0x2a8:rd:t6:t8 0x2a8:rt:t5:t7 0x2ac:rd:t7:t9 0x2ac:rt:t6:t8 0x2b0:rs:t7:t9 0x2d4:rt:t9:v1 0x2e0:rt:t0:v1 0x2ec:rt:t8:v1 0x2f8:rt:t8:v1 0x304:rt:t9:v1 0x30c:rt:t0:v1 0x35c:rt:t2:v1 0x368:rt:t3:v1 0x374:rt:t1:v1 0x380:rt:t1:v1 0x38c:rt:t2:v1 0x394:rt:t3:v1 0x3b8:rt:t4:t0 0x3bc:rs:t4:t0 0x3bc:rt:t4:t0 0x3c4:rs:t4:t0 0x3c4:rt:t5:t1 0x3c8:rs:t5:t1 0x3ec:rt:t7:v1 0x3f8:rt:t8:v1 0x404:rt:t6:v1 0x410:rt:t6:v1 0x41c:rt:t7:v1 0x428:rt:t8:v1 0x448:rt:t0:v1 0x454:rt:t1:v1 0x460:rt:t9:v1 0x46c:rt:t9:v1 0x478:rt:t0:v1 0x480:rt:t1:v1 0x524:rd:t5:t6 0x528:rs:t5:t6 0x534:rd:t6:t7 0x538:rt:t6:t7 0x540:rt:t8:t9 0x548:rd:t9:t0 0x548:rt:t8:t9 0x54c:rs:t9:t0 0x550:rt:t0:t2 0x558:rt:t0:t2 0x55c:rt:t1:t4 0x564:rs:t1:t4 0x564:rt:t2:t5 0x56c:rt:t2:t5 0x5a8:rt:t3:t6 0x5bc:rs:t3:t6 0x5bc:rt:t3:t6 0x5c4:rs:t3:t6 0x5c4:rt:t4:t7 0x5c8:rs:t4:t7 0x5e0:rt:t6:v1 0x5e4:rt:t5:v1 0x5f0:rt:t5:v1 0x5f8:rt:t6:v1 */
#include "types.h"

extern void sub_sub_step_up(void);
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
extern void zyotai_sprite_set_chk(void);
extern void serifu_run(void);
extern s32  get_g_zyotai_low(s32);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init(void);
extern void bunkasai_girl1_clubs3_5_srf_bank_set(void);

extern s16 now_serifu;
extern volatile u8 screen_w[];
extern s16 girl1_syoushin;
extern u8  girl1_basyo;
extern u8  now_girl;
extern u8  now_sub_sub_step;

void bunkasai_girl1_club4_97_scene(void) {
    s32 idx = now_sub_sub_step;

    s32 v0;
    s32 v1;

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
            v1 = 45;
            now_serifu = v1;
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
            bunkasai_girl1_clubs3_5_srf_bank_set();
        }
        break;
    case 9:
        serifu_run();
        break;
    case 10:
        v0 = get_g_zyotai_low(1);
        if (v0 == 4) {
            v1 = 112;
            now_serifu = v1;
        } else if ((*(volatile s32 *) &screen_w[512] << 29) >= 0) {
            v1 = 6;
            now_serifu = v1;
        } else if (((u32) girl1_basyo >> 4) == 0) {
            v1 = 10;
            now_serifu = v1;
        } else {
            v1 = 11;
            now_serifu = v1;
        }
        sub_sub_step_up();
        break;
    case 11:
        serifu_run();
        break;
    case 12:
        v0 = get_g_zyotai_low(1);
        if (v0 == 4) {
            goto_sub_sub_step(20);
        } else if (((u32) (*(volatile s32 *) &screen_w[512] << 29)
                    >> 31) == 1) {
            v0 = get_g_zyotai_low(1);
            switch (v0) {
            case 0:
            case 1:
                v1 = 114;
                now_serifu = v1;
                break;
            case 2:
                v1 = 115;
                now_serifu = v1;
                break;
            case 3:
                v1 = 116;
                now_serifu = v1;
                break;
            }
            sub_sub_step_up();
        } else {
            goto_sub_sub_step(14);
        }
        break;
    case 13:
        serifu_run();
        break;
    case 14:
        v0 = get_g_zyotai_low(1);
        switch (v0) {
        case 0:
        case 1:
            v1 = 117;
            now_serifu = v1;
            break;
        case 2:
            v1 = 118;
            now_serifu = v1;
            break;
        case 3:
            v1 = 119;
            now_serifu = v1;
            break;
        }
        sub_sub_step_up();
        break;
    case 15:
        serifu_run();
        break;
    case 16:
        if ((girl1_syoushin & 3) == 0) {
            v0 = get_g_zyotai_low(1);
            switch (v0) {
            case 0:
            case 1:
                v1 = 138;
                now_serifu = v1;
                break;
            case 2:
                v1 = 139;
                now_serifu = v1;
                break;
            case 3:
                v1 = 140;
                now_serifu = v1;
                break;
            }
        } else {
            v0 = get_g_zyotai_low(1);
            switch (v0) {
            case 0:
            case 1:
                v1 = 120;
                now_serifu = v1;
                break;
            case 2:
                v1 = 121;
                now_serifu = v1;
                break;
            case 3:
                v1 = 122;
                now_serifu = v1;
                break;
            }
        }
        sub_sub_step_up();
        break;
    case 17:
        serifu_run();
        break;
    case 18:
        if ((girl1_syoushin & 3) == 0) {
            goto_sub_sub_step(20);
        } else {
            v0 = get_g_zyotai_low(1);
            switch (v0) {
            case 0:
            case 1:
                v1 = 126;
                break;
            case 2:
                v1 = 127;
                break;
            case 3:
                v1 = 128;
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
    case 19:
        serifu_run();
        break;
    case 20:
        fade_down_main();
        break;
    case 21:
        v0 = get_g_zyotai_low(1);
        if (v0 == 4) {
            sub_sub_step_up();
        } else if ((girl1_syoushin & 3) == 0) {
            v0 = get_g_zyotai_low(1);
            if ((u32) v0 < 2) {
                v1 = 13;
                now_serifu = v1;
            } else {
                v1 = 12;
                now_serifu = v1;
            }
            sub_sub_step_up();
        } else {
            goto_sub_sub_step(23);
        }
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
