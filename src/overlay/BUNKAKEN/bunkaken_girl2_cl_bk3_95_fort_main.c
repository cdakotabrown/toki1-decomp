/* @SymbolStart: 0x80141664 */
/* @PostProc: patch_insn_reg_field 0x208:rt:v0:t7 0x20c:rs:v0:t7 0x20c:rt:v0:t7 0x210:rt:t8:t0 0x214:rd:t7:t9 0x214:rt:v0:t7 0x218:rs:t7:t9 0x230:rs:t8:t0 0x230:rt:t8:t1 0x234:rt:t0:t3 0x238:rs:t8:t1 0x238:rt:t9:t2 0x23c:rt:t9:t2 0x248:rt:t0:t3 0x27c:rt:t1:t4 0x280:rt:t1:t4 0x288:rs:t1:t4 0x290:rt:t2:t5 0x29c:rt:t2:t5 0x2a0:rt:t3:t6 0x2ac:rt:t3:t6 0x2b0:rt:t4:t7 0x2bc:rt:t4:t7 0x2c0:rt:t5:t8 0x2c8:rt:t5:t8 0x2cc:rt:t6:t9 0x2d0:rs:t6:t9 0x2d0:rt:t6:t9 0x2d8:rs:t6:t9 0x2dc:rt:t7:t0 0x2e4:rt:t7:t0 0x344:rt:t8:t1 0x348:rs:t8:t1 0x348:rt:t8:t1 0x34c:rt:t9:t2 0x350:rs:t9:t2 0x350:rt:t9:t2 0x358:rt:t8:t1 0x360:rs:t9:t2 0x360:rt:t0:t3 0x364:rt:t0:t3 0x3ac:rt:t1:t4 0x3b0:rs:t1:t4 0x3b0:rt:t1:t4 0x3b8:rs:t1:t4 0x3e8:rt:t2:t5 0x3ec:rs:t2:t5 0x418:rt:t4:t7 0x424:rt:t5:t8 0x430:rt:t3:t6 0x43c:rt:t3:t6 0x448:rt:t4:t7 0x450:rt:t5:t8 0x464:rt:t6:t9 0x468:rs:t6:t9 0x468:rt:t6:t9 0x470:rs:t6:t9 0x474:rt:t7:t0 0x478:rs:t7:t0 0x478:rt:t7:t0 0x480:rs:t7:t0 0x480:rt:t8:t1 0x488:rt:t8:t1 0x4c0:rt:t9:t2 0x4c4:rs:t9:t2 0x4c4:rt:t9:t2 0x4cc:rs:t9:t2 0x274:rd:t1:t4 */
/* @SymbolSize: 1324 */
/* @RodataBase: 0x8015463C */
/* Address: 0x8014166C */
/* Size: 1316 bytes (329 insns) */
#include "types.h"

extern s32 sub_sub_step_up(void);
extern s32 goto_sub_step(s32);
extern s32 goto_sub_sub_step(s32);
extern s32 device_cmd_send(s32);
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
extern s32 zyotai_sprite_set_chk(void);
extern s32 serifu_run(void);
extern s32 get_g_zyotai_low(s32);
extern s32 bunkaken_girl2_cl_bank_init(void);
extern s32 bunkaken_sub_reset(void);
extern void bunkaken_basyo_init3(void);

extern u8 girl2_flag;
extern s16 now_serifu;
extern u8 now_girl;
extern u8 now_sub_step;
extern u8 now_sub_sub_step;
extern s32 bunka_girl2;
extern s32 bunka_aisyou;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;

void bunkaken_girl2_cl_bk3_95_fort_main(void) {
    s32 t6 = now_sub_sub_step;
    s32 v0;
    s32 a;
    s32 b;

    switch ((u32)t6) {
    case 0:
        value_cache_find_or_load(16645, 1);
        sub_sub_step_up();
        break;
    case 1:
        fade_out_step();
        break;
    case 2:
        v0 = get_CD_State2();
        if ((v0 & 1) == 0) {
            break;
        }
        bunkaken_basyo_init3();
        v0 = dec_bg_init(1);
        indexed_byte_lsb_set(1, v0);
        bustup_cd_read(21038);
        sub_sub_step_up();
        break;
    case 3:
        fade_in_step();
        break;
    case 4:
        v0 = get_CD_State2();
        if ((v0 & 1) == 0) {
            break;
        }
        bustup_ptr_init();
        now_girl = 2;
        zyotai_sprite_set_chk();
        cd_read_start2(9, 0x801E8000, 19000);
        break;
    case 5:
        v0 = get_g_zyotai_low(2);
        if (v0 == 4 || bunka_girl2 == 2) {
            sub_sub_step_up();
        } else {
            fade_up_main();
        }
        break;
    case 6:
        v0 = get_CD_State2();
        if ((v0 & 1) == 0) {
            break;
        }
        bunkaken_girl2_cl_bank_init();
        v0 = get_g_zyotai_low(2);
        switch ((u32)v0) {
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
            goto_sub_sub_step(10);
            break;
        }
        if (bunka_girl2 == 2) {
            goto_sub_sub_step(10);
        }
        break;
    case 7:
        serifu_run();
        break;
    case 8:
        v0 = *(s32 *)&girl2_flag;
        if ((v0 << 29) < 0) {
            goto_sub_sub_step(10);
        } else {
            girl2_flag = girl2_flag | 4;
            now_serifu = 4;
            sub_sub_step_up();
        }
        break;
    case 9:
        serifu_run();
        break;
    case 10:
        v0 = get_g_zyotai_low(2);
        switch ((u32)v0) {
        case 0:
        case 1:
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
        if (bunka_girl2 == 2) {
            now_serifu = 32;
        }
        sub_sub_step_up();
        break;
    case 11:
        serifu_run();
        break;
    case 12:
        v0 = get_g_zyotai_low(2);
        if (v0 == 4) {
            sub_sub_step_up();
        } else {
            fade_down_main();
        }
        break;
    case 13:
        device_cmd_send(514);
        a = now_sub_step;
        b = now_sub_sub_step;
        bunka_ret_sub_step = a;
        bunka_ret_sub_sub_step = b + 1;
        goto_sub_step(11);
        break;
    case 14:
        cd_read_start2(9, 0x801E8000, 19000);
        sub_sub_step_up();
        break;
    case 15:
        v0 = get_g_zyotai_low(2);
        if (v0 == 4 || bunka_girl2 == 2) {
            sub_sub_step_up();
        } else {
            fade_up_main();
        }
        break;
    case 16:
        v0 = get_CD_State2();
        if ((v0 & 1) == 0) {
            break;
        }
        bunkaken_girl2_cl_bank_init();
        v0 = get_g_zyotai_low(2);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = 33;
            break;
        case 2:
            now_serifu = 34;
            break;
        case 3:
            now_serifu = 35;
            break;
        }
        v0 = get_g_zyotai_low(2);
        if (v0 == 4 || bunka_girl2 == 2) {
            now_serifu = bunka_aisyou + 36;
        }
        break;
    case 17:
        serifu_run();
        break;
    case 18:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 19:
        v0 = get_g_zyotai_low(2);
        if (v0 == 4 || bunka_girl2 == 2) {
            sub_sub_step_up();
        } else {
            fade_down_main();
        }
        break;
    case 20:
        fade_out_step();
        break;
    case 21:
        serifu_run();
        break;
    case 22:
        bunkaken_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
