/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:130 */
/* @SymbolStart: 0x801569BC */
/* @SymbolSize: 1712 */
/* @RodataBase: 0x8015FB94 */
/* @PostProc: swap_instructions --offset1 0x3c0 --offset2 0x3c4 */
/* @PostProc: patch_insn_reg_field 0xd8:rt:t9:v1 0xe4:rt:t9:v1 0xe8:rt:t0:t9 0xf4:rt:t0:t9 0x14c:rt:t1:t0 0x150:rs:t1:t0 0x168:rd:t2:t1 0x170:rt:t2:t1 0x174:rt:t2:t1 0x17c:rs:t2:t1 0x184:rt:t3:v1 0x190:rt:t3:v1 0x194:rt:t4:v1 0x1a0:rt:t4:v1 0x1a4:rt:t5:v1 0x1b0:rt:t5:v1 0x1b4:rt:t6:v1 0x1bc:rt:t6:v1 0x1f0:rd:t7:t2 0x1f8:rt:t7:t2 0x1fc:rt:t7:t2 0x204:rs:t7:t2 0x20c:rt:t8:v1 0x218:rt:t8:v1 0x21c:rt:t9:v1 0x228:rt:t9:v1 0x22c:rt:t0:v1 0x238:rt:t0:v1 0x23c:rt:t1:v1 0x244:rt:t1:v1 0x2b4:rt:t2:t3 0x2b8:rs:t2:t3 0x2b8:rt:t2:t3 0x2bc:rt:t3:t4 0x2c0:rs:t3:t4 0x2c0:rt:t3:t4 0x2c8:rt:t2:t3 0x2d0:rs:t3:t4 0x2d0:rt:t4:t5 0x2d8:rt:t4:t5 0x344:rt:t5:t6 0x348:rs:t5:t6 0x360:rd:t6:t7 0x368:rt:t6:t7 0x36c:rt:t6:t7 0x374:rs:t6:t7 0x37c:rt:t7:v1 0x388:rt:t7:v1 0x38c:rt:t8:v1 0x398:rt:t8:v1 0x39c:rt:t9:v1 0x3a8:rt:t9:v1 0x3ac:rt:t0:v1 0x3b4:rt:t0:v1 0x3b8:rt:t1:v1 0x3bc:rt:t2:t8 0x3c0:rs:t1:v1 0x3c0:rt:t1:v1 0x3c4:rs:t2:t8 0x3c4:rt:t2:t8 0x3cc:rd:t3:t0 0x3cc:rs:t1:v1 0x3cc:rt:t2:t8 0x3d4:rt:t3:t0 0x3f8:rt:t4:t1 0x3fc:rs:t4:t1 0x3fc:rt:t4:t1 0x404:rs:t4:t1 0x404:rt:t5:t2 0x408:rs:t5:t2 0x420:rd:t6:t3 0x428:rt:t6:t3 0x42c:rt:t6:t3 0x434:rs:t6:t3 0x43c:rt:t7:v1 0x448:rt:t7:v1 0x44c:rt:t8:v1 0x458:rt:t8:v1 0x45c:rt:t9:v1 0x468:rt:t9:v1 0x48c:rd:t0:t4 0x494:rt:t0:t4 0x498:rt:t0:t4 0x4a0:rs:t0:t4 0x4a8:rt:t1:v1 0x4b4:rt:t1:v1 0x4b8:rt:t2:v1 0x4c4:rt:t2:v1 0x4c8:rt:t3:v1 0x4d4:rt:t3:v1 0x4f8:rt:t4:t5 0x4fc:rs:t4:t5 0x4fc:rt:t4:t5 0x504:rs:t4:t5 0x504:rt:t5:t6 0x508:rs:t5:t6 0x52c:rt:t0:t2 0x574:rt:t6:t7 0x57c:rd:t7:t9 0x57c:rt:t6:t7 0x580:rs:t7:t9 0x58c:rd:t8:t0 0x590:rt:t8:t0 0x594:rs:t0:t2 0x594:rt:t0:t2 0x5a0:rd:t1:t3 0x5a0:rt:t0:t2 0x5a4:rs:t1:t3 0x5a8:rt:t2:t5 0x5b0:rt:t2:t5 0x5e4:rt:t3:t6 0x5e8:rs:t3:t6 0x5e8:rt:t3:t6 0x5f0:rs:t3:t6 0x5f0:rt:t4:t7 0x5f4:rs:t4:t7 0x60c:rt:t6:v1 0x610:rt:t5:v1 0x61c:rt:t5:v1 0x624:rt:t6:v1 0x638:rt:t7:t9 0x640:rs:t7:t9 0x640:rt:t8:t0 0x648:rt:t8:t0 */
/* Address: 0x801569C4 */
/* Size: 1704 bytes (426 insns) */
/* @SymbolSize: 1712 (head absorbed) */
#include "types.h"

extern void sub_step_up(void);
extern void sub_sub_step_up(void);
extern void goto_sub_sub_step(s32);
extern void device_cmd_send(s32);
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
extern u8  now_sub_step;
extern u8  now_sub_sub_step;
extern s32 bunka_aisyou;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;

void bunkasai_girl1_club3_95_scene(void) {
    s32 idx = now_sub_sub_step;

    s32 v0;
    s32 v1;

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
            bustup_cd_read(23873);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in_step();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            v1 = 42;
            now_serifu = v1;
            bustup_ptr_init();
            now_girl = 1;
            zyotai_sprite_set_chk();
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
        switch (v0) {
        case 0:
        case 1:
            v1 = 14;
            now_serifu = v1;
            break;
        case 2:
            v1 = 15;
            now_serifu = v1;
            break;
        case 3:
            v1 = 16;
            now_serifu = v1;
            break;
        case 4:
            v1 = 17;
            now_serifu = v1;
            break;
        }
        sub_sub_step_up();
        break;
    case 11:
        serifu_run();
        break;
    case 12:
        fade_down_main();
        break;
    case 13:
        if (get_g_zyotai_low(1) == 4) {
            serifu_run();
        } else {
            sub_sub_step_up();
        }
        break;
    case 14:
        device_cmd_send(514);
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        sub_step_up();
        break;
    case 15:
        cd_read_start2(18, 0x801E8000, 19095);
        sub_sub_step_up();
        break;
    case 16:
        if (get_g_zyotai_low(1) == 4) {
            sub_sub_step_up();
        } else {
            fade_up_main();
        }
        break;
    case 17:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(1);
            switch (v0) {
            case 0:
            case 1:
                v1 = 19;
                now_serifu = v1;
                break;
            case 2:
                v1 = 22;
                now_serifu = v1;
                break;
            case 3:
                v1 = 25;
                now_serifu = v1;
                break;
            case 4:
                v1 = 28;
                now_serifu = v1;
                break;
            }
            now_serifu = now_serifu + bunka_aisyou;
            bunkasai_girl1_clubs3_5_srf_bank_set();
        }
        break;
    case 18:
        serifu_run();
        break;
    case 19:
        sub_sub_step_up();
        if ((girl1_syoushin & 3) == 0) {
            v0 = get_g_zyotai_low(1);
            switch (v0) {
            case 0:
            case 1:
                v1 = 46;
                now_serifu = v1;
                break;
            case 2:
                v1 = 47;
                now_serifu = v1;
                break;
            case 3:
                v1 = 48;
                now_serifu = v1;
                break;
            case 4:
                goto_sub_sub_step(26);
                break;
            }
        } else {
            v0 = get_g_zyotai_low(1);
            switch (v0) {
            case 0:
            case 1:
                v1 = 31;
                now_serifu = v1;
                break;
            case 2:
                v1 = 32;
                now_serifu = v1;
                break;
            case 3:
                v1 = 33;
                now_serifu = v1;
                break;
            case 4:
                goto_sub_sub_step(26);
                break;
            }
        }
        break;
    case 20:
        serifu_run();
        break;
    case 21:
        if ((girl1_syoushin & 3) == 0) {
            goto_sub_sub_step(23);
        } else {
            v0 = get_g_zyotai_low(1);
            switch (v0) {
            case 0:
            case 1:
                v1 = 34;
                break;
            case 2:
                v1 = 35;
                break;
            case 3:
                v1 = 36;
                break;
            default:
                v1 = now_serifu;
                break;
            }
            if ((*(volatile s32 *) &screen_w[512] << 29) >= 0) {
                v1 = (s16) (v1 + 3);
            }
            now_serifu = v1;
            if (((u32) girl1_basyo >> 4) == 1) {
                now_serifu = v1 + 6;
            }
            sub_sub_step_up();
        }
        break;
    case 22:
        serifu_run();
        break;
    case 23:
        fade_down_main();
        break;
    case 24:
        if ((girl1_syoushin & 3) == 0) {
            if ((u32) get_g_zyotai_low(1) < 2U) {
                v1 = 13;
                now_serifu = v1;
            } else {
                v1 = 12;
                now_serifu = v1;
            }
            sub_sub_step_up();
        } else {
            screen_w[512] = screen_w[512] | 4;
            goto_sub_sub_step(26);
        }
        break;
    case 25:
        serifu_run();
        break;
    case 26:
        fade_out_step();
        break;
    case 27:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 28:
        serifu_run();
        break;
    case 29:
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
