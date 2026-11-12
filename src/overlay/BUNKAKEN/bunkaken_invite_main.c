/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=all */
/* @SymbolStart: 0x8013B510 */
/* Address: 0x8013B518 */
/* Size: 2156 bytes (539 insns) */
/* @SymbolSize: 2164 */
/* @RodataBase: 0x80153E10 */
/* @PostProc: patch_insn_reg_field 0x12c:rt:v0:v1 0x134:rs:v0:v1 0x138:rs:v0:v1 0x154:rt:v0:v1 0x368:rd:t6:t8 0x36c:rs:t6:t8 0x36c:rt:t7:t0 0x374:rt:t7:t0 0x390:rt:t8:t9 0x394:rs:t8:t9 0x394:rt:t8:t1 0x39c:rs:t8:t1 0x39c:rt:t0:t2 0x3a0:rt:t0:t2 0x3a8:rt:t9:t3 0x3b0:rt:t9:t3 0x3dc:rt:t1:t4 0x3e0:rs:t1:t4 0x3e0:rt:t1:t4 0x3e8:rs:t1:t4 0x420:rt:t2:t5 0x424:rs:t2:t5 0x424:rt:t2:t5 0x428:rt:t3:t6 0x42c:rs:t2:t5 0x444:rs:t3:t6 0x444:rt:t3:t6 0x44c:rd:t4:t0 0x44c:rt:t3:t6 0x450:rs:t4:t0 0x450:rt:t5:t9 0x458:rt:t5:t9 0x484:rt:t6:t1 0x488:rs:t6:t1 0x488:rt:t6:t1 0x490:rd:t7:t2 0x490:rt:t6:t1 0x494:rt:t7:t2 0x4c4:rt:t8:t3 0x4cc:rt:t8:t3 0x514:rt:t0:t4 0x520:rt:t3:t8 0x52c:rt:t7:t2 0x538:rs:t0:t4 0x538:rt:t0:t4 0x540:rd:t9:t5 0x540:rt:t0:t4 0x544:rs:t9:t5 0x560:rt:t2:t7 0x564:rs:t3:t8 0x564:rt:t3:t8 0x56c:rd:t4:t0 0x56c:rt:t3:t8 0x570:rs:t4:t0 0x574:rt:t6:t1 0x578:rt:t5:t9 0x584:rt:t5:t9 0x590:rt:t6:t1 0x594:rs:t7:t2 0x594:rt:t7:t2 0x59c:rd:t8:t3 0x59c:rt:t7:t2 0x5a0:rs:t8:t3 0x5a4:rt:t9:t5 0x5a8:rt:t0:t4 0x5b4:rt:t0:t4 0x5c0:rt:t9:t5 0x5d4:rt:t2:t7 0x5d8:rd:t1:t6 0x5dc:rd:t1:t6 0x5dc:rs:t1:t6 0x5e0:rd:t1:t6 0x5e0:rt:t1:t6 0x5e4:rs:t2:t7 0x5e4:rt:t2:t7 0x5e8:rs:t1:t6 0x5e8:rt:t2:t7 0x5ec:rt:t3:t0 0x5f4:rs:t3:t0 0x5f4:rt:t4:t9 0x5fc:rt:t4:t9 0x618:rt:t5:t1 0x624:rt:t5:t1 0x650:rt:t6:t2 0x654:rs:t6:t2 0x654:rt:t6:t2 0x65c:rs:t6:t2 0x6b4:rt:t7:t3 0x6b8:rs:t7:t3 0x6b8:rt:t7:t3 0x6c0:rs:t7:t3 0x6e4:rt:t8:t4 0x6f0:rt:t2:t8 0x6fc:rt:t6:t2 0x708:rs:t8:t4 0x708:rt:t8:t4 0x710:rd:t0:t5 0x710:rt:t8:t4 0x714:rs:t0:t5 0x730:rt:t1:t7 0x734:rs:t2:t8 0x734:rt:t2:t8 0x73c:rd:t3:t0 0x73c:rt:t2:t8 0x740:rs:t3:t0 0x744:rt:t5:t1 0x748:rt:t4:t9 0x754:rt:t4:t9 0x760:rt:t5:t1 0x764:rs:t6:t2 0x764:rt:t6:t2 0x76c:rd:t7:t3 0x76c:rt:t6:t2 0x770:rs:t7:t3 0x774:rt:t0:t5 0x778:rt:t8:t4 0x784:rt:t8:t4 0x790:rt:t0:t5 0x7d4:rt:t1:t7 0x7d8:rd:t9:t6 0x7dc:rd:t9:t6 0x7dc:rs:t9:t6 0x7e0:rd:t9:t6 0x7e0:rt:t9:t6 0x7e4:rd:t1:t7 0x7e4:rs:t1:t7 0x7e4:rt:t9:t6 0x7e8:rs:t1:t7 0x7e8:rt:t1:t7 0x7f0:rd:t2:t8 0x7f0:rt:t1:t7 0x7f4:rt:t2:t8 0x828:rt:t3:t0 0x834:rt:t3:t0 */
/* @PostProc: patch_insn_reg_field 0x53c:rt:t2:t7 0x54c:rt:t1:t6 0x558:rt:t1:t6 0x70c:rt:t1:t7 0x71c:rt:t9:t6 0x728:rt:t9:t6 */
#include "types.h"

extern s32 sub_sub_step_up(void);
extern void goto_sub_step(s32);
extern void goto_sub_sub_step(s32);
extern s32 get_CD_State2(void);
extern void cd_read_start2(s32, s32, s32);
extern void game_reset(void);
extern void bustup_kuchi_ani(void);
extern void bustup_ptr_init(void);
extern void fade_up_main(void);
extern void fade_down_main(void);
extern void menu2_main(void);
extern void bustup_face_change(s32);
extern void zyotai_sprite_set_chk(void);
extern void date_name_set(s32);
extern void serifu_run(void);
extern void bustup_load_step(s32);
extern s32 bunkaken_pick_judge(void);
extern void bunkaken_ptr_copy(void);

extern s32 bunka_girl;
extern s32 serifu_no;
extern s32 bunka_girl2;
extern s32 bunka_bukatu;

extern volatile s16 now_serifu;
extern volatile s16 now_serifu_in;

extern u8 now_girl;
extern u8 girl0_flag;
extern u8 girl0_basyo;
extern u8 girl1_basyo;
extern u8 girl2_basyo;
extern u8 girl3_basyo;
extern u8 screen_w[][56];

extern s32 bunkaken_bust_size_tbl[];
extern u8 bunkaken_cd_cnt_tbl[];
extern s32 bunkaken_cd_lba_tbl[];
extern void (*bunkaken_ptr_select_tbl[])(void);
extern u8 bunkaken_face_tbl[];
extern u8 now_sub_sub_step;

void bunkaken_invite_main(void) {
    s32 arg = now_sub_sub_step;
    s32 v0;
    s32 v1;

    switch ((u32)arg) {
    case 0:
        if (bunkaken_pick_judge()) {
            sub_sub_step_up();
        } else {
            goto_sub_step(1);
        }
        break;
    case 1:
        bustup_load_step(bunkaken_bust_size_tbl[bunka_girl]);
        break;
    case 2:
        bustup_ptr_init();
        v1 = bunka_girl;
        if (v1 == 11) {
            now_girl = 0;
        } else {
            now_girl = v1;
        }
        zyotai_sprite_set_chk();
        v1 = bunka_girl;
        cd_read_start2(bunkaken_cd_cnt_tbl[v1], 0x801E8000, bunkaken_cd_lba_tbl[v1]);
        break;
    case 3:
        fade_up_main();
        break;
    case 4:
        v0 = get_CD_State2();
        if ((v0 & 1) == 0) {
            return;
        }
        v1 = bunka_girl;
        if (v1 == 11) {
            date_name_set(0);
        } else {
            bunka_girl = v1;
            date_name_set(v1 & 0xFF);
        }
        bunkaken_ptr_select_tbl[bunka_girl]();
        bunkaken_ptr_copy();
        now_serifu = 0;
        now_serifu_in = 0;
        sub_sub_step_up();
        break;
    case 5:
        serifu_run();
        break;
    case 6:
        switch ((u32)bunka_girl) {
        case 0:
            goto_sub_sub_step(17);
            break;
        case 1:
        case 2:
        case 3:
            goto_sub_sub_step(32);
            break;
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
            goto_sub_sub_step(7);
            break;
        }
        break;
    case 7:
        menu2_main();
        break;
    case 8:
        if (serifu_no != 0) {
            v1 = bunka_girl;
            bustup_face_change(bunkaken_face_tbl[v1]);
            bustup_kuchi_ani();
            v1 = bunka_girl;
            if (v1 == 11) {
                bunka_girl2 = 0;
            } else {
                bunka_girl2 = v1;
            }
        }
        sub_sub_step_up();
        break;
    case 9:
        serifu_run();
        break;
    case 10:
        if (serifu_no == 0) {
            v1 = bunka_girl;
            if (v1 == 8 || v1 == 9) {
                goto_sub_sub_step(14);
            } else {
                goto_sub_step(3);
            }
        } else {
            sub_sub_step_up();
        }
        break;
    case 11:
        fade_down_main();
        break;
    case 12:
        serifu_run();
        break;
    case 13:
        goto_sub_step(1);
        break;
    case 14:
        now_serifu = 8;
        sub_sub_step_up();
        break;
    case 15:
        serifu_run();
        break;
    case 16:
        goto_sub_step(3);
        break;
    case 17:
        now_serifu = ((u32)girl0_basyo >> 4) + 1;
        sub_sub_step_up();
        break;
    case 18:
        serifu_run();
        break;
    case 19:
        girl0_flag |= 4;
        now_serifu = 7;
        sub_sub_step_up();
        break;
    case 20:
        serifu_run();
        break;
    case 21:
        menu2_main();
        break;
    case 22:
        if (serifu_no != 0) {
            bustup_face_change(3);
            bustup_kuchi_ani();
        }
        sub_sub_step_up();
        break;
    case 23:
        serifu_run();
        break;
    case 24:
        if (serifu_no != 0) {
            goto_sub_sub_step(28);
        } else {
            now_serifu = ((u32)girl0_basyo >> 4) + 9;
            sub_sub_step_up();
        }
        break;
    case 25:
        fade_down_main();
        break;
    case 26:
        serifu_run();
        break;
    case 27:
        bunka_bukatu = (u32)girl0_basyo >> 4;
        bunka_girl = 255;
        goto_sub_step(4);
        break;
    case 28:
        fade_down_main();
        break;
    case 29:
        now_serifu = 15;
        bunka_girl2 = 0;
        sub_sub_step_up();
        break;
    case 30:
        serifu_run();
        break;
    case 31:
        goto_sub_step(1);
        break;
    case 32:
        v1 = bunka_girl;
        switch (v1) {
        case 1:
            if (((u32)girl1_basyo >> 4) == 0) {
                now_serifu = 1;
            } else {
                now_serifu = 2;
            }
            break;
        case 2:
            if (((u32)girl2_basyo >> 4) == 2) {
                now_serifu = 1;
            } else {
                now_serifu = 2;
            }
            break;
        case 3:
            if (((u32)girl3_basyo >> 4) == 4) {
                now_serifu = 1;
            } else {
                now_serifu = 2;
            }
            break;
        default:
            game_reset();
            break;
        }
        v1 = bunka_girl;
        screen_w[v1][456] |= 4;
        sub_sub_step_up();
        break;
    case 33:
        serifu_run();
        break;
    case 34:
        now_serifu = 3;
        sub_sub_step_up();
        break;
    case 35:
        serifu_run();
        break;
    case 36:
        menu2_main();
        break;
    case 37:
        if (serifu_no != 0) {
            if (bunka_girl == 2) {
                bustup_face_change(4);
            } else {
                bustup_face_change(3);
            }
            bustup_kuchi_ani();
        }
        sub_sub_step_up();
        break;
    case 38:
        serifu_run();
        break;
    case 39:
        if (serifu_no != 0) {
            goto_sub_sub_step(43);
        } else {
            v1 = bunka_girl;
            switch (v1) {
            case 1:
                if (((u32)girl1_basyo >> 4) == 0) {
                    now_serifu = 5;
                } else {
                    now_serifu = 6;
                }
                break;
            case 2:
                if (((u32)girl2_basyo >> 4) == 2) {
                    now_serifu = 5;
                } else {
                    now_serifu = 6;
                }
                break;
            case 3:
                if (((u32)girl3_basyo >> 4) == 4) {
                    now_serifu = 5;
                } else {
                    now_serifu = 6;
                }
                break;
            default:
                game_reset();
                break;
            }
            sub_sub_step_up();
        }
        break;
    case 40:
        fade_down_main();
        break;
    case 41:
        serifu_run();
        break;
    case 42:
        v1 = bunka_girl;
        bunka_bukatu = (u32)screen_w[v1][458] >> 4;
        bunka_girl = 255;
        goto_sub_step(4);
        break;
    case 43:
        fade_down_main();
        break;
    case 44:
        now_serifu = 7;
        bunka_girl2 = bunka_girl;
        sub_sub_step_up();
        break;
    case 45:
        serifu_run();
        break;
    case 46:
        goto_sub_step(1);
        break;
    default:
        game_reset();
        break;
    }
}
