/* master_bukatu_judge - MASTER overlay, Address: 0x80139CD0, Size: 2740 bytes (685 insns) */
/* @CompileFlags: -Wo,-loopunroll,0 -Wc,-pic0 */
/* @SymbolStart: 0x80139CC8 */
/* @SymbolSize: 2748 */
/* @RodataBase: 0x8013C17C */
/* @PostProc: remap_stack_slots --map '64:52,44:60,36:48' */
/* @PostProc: swap_instructions --offset1 0x56c --offset2 0x570 */
/* @PostProc: swap_instructions --offset1 0x8d0 --offset2 0x8d4 */
/* @PostProc: swap_instructions --offset1 0x8d4 --offset2 0x8d8 */
/* @PostProc: swap_instructions --offset1 0x8d8 --offset2 0x8e0 */
/* @PostProc: swap_instructions --offset1 0x92c --offset2 0x930 */
/* @PostProc: swap_instructions --offset1 0x930 --offset2 0x934 */
/* @PostProc: swap_instructions --offset1 0x934 --offset2 0x93c */
/* @PostProc: swap_instructions --offset1 0xa1c --offset2 0xa20 */
/* @PostProc: swap_instructions --offset1 0xa20 --offset2 0xa24 */
/* @PostProc: swap_instructions --offset1 0xa24 --offset2 0xa2c */
/* @PostProc: swap_instructions --offset1 0xa88 --offset2 0xa8c */
/* @PostProc: swap_instructions --offset1 0xa8c --offset2 0xa90 */
/* @PostProc: swap_instructions --offset1 0xa90 --offset2 0xa98 */
/* @PostProc: patch_insn_reg_field 0x304:rd:t8:t7 0x308:rd:t9:t8 0x308:rt:t8:t7 0x30c:rt:t9:t8 0x31c:rd:t1:t9 0x320:rt:t1:t9 0x408:rt:v1:a0 0x410:rs:v1:a0 0x41c:rt:a0:v1 0x424:rt:v1:a0 0x430:rt:a0:v1 0x46c:rt:v0:at 0x470:rs:v0:at 0x474:rs:v0:at 0x488:rt:t3:t2 0x490:rt:t3:t2 0x498:rt:t4:t3 0x4a0:rt:t5:t4 0x4a4:rt:t5:t4 0x4a8:rt:t4:t3 0x4b8:rt:t6:t5 0x4c4:rt:t6:t5 0x4d8:rt:t7:t6 0x4dc:rs:t7:t6 0x4dc:rt:t7:t6 0x4e4:rd:t8:t7 0x4e4:rt:t7:t6 0x4e8:rs:t8:t7 0x4f4:rt:t9:t8 0x500:rt:t9:t8 0x524:rt:t1:t9 0x52c:rt:t1:t9 0x534:rt:t2:t1 0x538:rt:t2:t1 0x540:rt:t3:t2 0x544:rt:t3:t2 0x54c:rt:t4:t3 0x550:rt:t4:t3 0x558:rt:t5:t4 0x55c:rt:t5:t4 0x564:rt:t6:t5 0x568:rt:t6:t5 0x56c:rt:t7:v0 0x578:rt:t7:v0 0x588:rt:t1:t8 0x58c:rt:t2:t9 0x590:rt:t3:t1 0x594:rs:t3:t1 0x594:rt:t3:t1 0x598:rt:t4:t2 0x59c:rs:t4:t2 0x59c:rt:t4:t2 0x5a0:rt:t5:t3 0x5a4:rs:t5:t3 0x5a4:rt:t5:t3 0x5a8:rt:t6:t4 0x5ac:rs:t6:t4 0x5ac:rt:t6:t4 0x5b0:rd:t8:t6 0x5b4:rs:t8:t6 0x5b4:rt:t9:t7 0x5b8:rt:t9:t7 0x5c0:rt:t1:t8 0x5c8:rt:t2:t9 0x5d0:rt:t3:t1 0x5d8:rt:t4:t2 0x5e0:rt:t5:t3 0x5e8:rt:t6:t4 0x600:rt:t7:t5 0x604:rs:t7:t5 0x604:rt:t7:t5 0x608:rt:v1:a0 0x60c:rs:t7:t5 0x614:rs:v1:a0 0x614:rt:v1:a0 0x61c:rs:v1:a0 0x628:rs:v1:a0 0x62c:rt:t8:t6 0x630:rs:t8:t6 0x630:rt:t8:t6 0x638:rd:t9:t7 0x638:rt:t8:t6 0x63c:rs:t9:t7 0x640:rt:t1:t8 0x654:rt:t1:t8 0x65c:rt:t1:t8 0x664:rt:t2:t9 0x668:rt:t2:t9 0x66c:rt:t3:t1 0x674:rd:t3:t1 0x674:rs:t3:t1 0x674:rt:v1:a0 0x678:rs:t3:t1 0x678:rt:t3:t1 0x680:rt:a0:v1 0x688:rs:a0:v1 0x688:rt:t3:t1 0x69c:rt:t4:t2 0x6a4:rt:t4:t2 0x6a8:rs:v1:a0 0x6a8:rt:t5:t3 0x6b4:rt:t5:t3 0x6c0:rt:t7:t5 0x6d0:rt:t6:t4 0x6d8:rt:t6:t4 0x6ec:rs:t7:t5 0x6ec:rt:t7:t5 0x6f4:rs:t7:t5 0x6f4:rt:t8:t6 0x6fc:rt:t8:t6 0x71c:rd:t9:t7 0x720:rs:t9:t7 0x72c:rs:t9:t7 0x738:rs:t9:t7 0x744:rs:t9:t7 0x758:rt:t1:t8 0x764:rt:t2:t9 0x770:rt:t1:t8 0x77c:rt:t2:t9 0x784:rt:t3:t1 0x790:rt:t4:t2 0x79c:rt:t3:t1 0x7a8:rt:t4:t2 0x7b0:rt:t5:t3 0x7bc:rt:t6:t4 0x7c8:rt:t5:t3 0x7d4:rt:t6:t4 0x7dc:rt:t7:t5 0x7e8:rt:t8:t6 0x7f4:rt:t7:t5 0x800:rt:t8:t6 0x830:rt:t9:t7 0x844:rt:t9:t7 0x860:rt:t1:t8 0x864:rs:t1:t8 0x864:rt:t1:t8 0x86c:rs:t1:t8 0x878:rt:t8:t1 0x888:rt:t2:t9 0x88c:rs:t2:t9 0x88c:rt:t2:t9 0x898:rt:a3:v1 0x89c:rs:t2:t9 0x89c:rt:t3:t1 0x8a0:rt:t3:t1 0x8a4:rs:a3:v1 0x8a4:rt:a3:v1 0x8ac:rt:v1:t4 0x8b0:rt:a0:t6 0x8b4:rt:a1:t8 0x8b8:rt:a2:t2 0x8c0:rs:v1:t4 0x8c0:rt:t6:t5 0x8c4:rs:a0:t6 0x8c4:rt:t5:t7 0x8c8:rs:a1:t8 0x8c8:rt:t4:t9 0x8cc:rs:a2:t2 0x8cc:rt:t7:t3 0x8d0:rt:t4:t9 0x8d4:rt:t5:t7 0x8d8:rt:t6:t5 0x8dc:rt:a3:v1 0x8e0:rt:t7:t3 0x8ec:rs:t8:t1 0x8ec:rt:t8:t1 0x8f4:rt:a3:v1 0x8f8:rs:t8:t1 0x8f8:rt:t9:t2 0x8fc:rt:t9:t2 0x900:rs:a3:v1 0x900:rt:a3:v1 0x908:rt:v1:t5 0x90c:rt:a0:t7 0x910:rt:a1:t9 0x914:rt:a2:t3 0x91c:rs:v1:t5 0x91c:rt:t3:t6 0x920:rs:a0:t7 0x920:rt:t2:t8 0x924:rs:a1:t9 0x928:rs:a2:t3 0x930:rt:t2:t8 0x934:rt:t3:t6 0x938:rt:a3:v1 0x958:rt:t5:t2 0x95c:rs:t5:t2 0x95c:rt:t5:t2 0x964:rd:t6:t3 0x964:rt:t5:t2 0x968:rs:t6:t3 0x98c:rt:t8:t5 0x990:rs:t8:t5 0x990:rt:t8:t5 0x994:rt:t1:t7 0x998:rd:t9:t6 0x998:rt:t8:t5 0x99c:rd:t9:t6 0x99c:rs:t9:t6 0x99c:rt:t8:t5 0x9a0:rt:t7:t4 0x9a8:rd:t9:t6 0x9a8:rt:t9:t6 0x9ac:rs:t1:t7 0x9ac:rt:t1:t7 0x9b4:rt:t7:t4 0x9b8:rs:t9:t6 0x9b8:rt:t1:t7 0x9c0:rt:t2:t8 0x9c8:rs:t2:t8 0x9c8:rt:t3:t9 0x9d0:rt:t3:t9 0x9d4:rt:t4:t1 0x9d8:rs:t4:t1 0x9d8:rt:t4:t1 0x9e4:rt:a3:v1 0x9e8:rs:t4:t1 0x9e8:rt:t5:t2 0x9ec:rt:t5:t2 0x9f0:rs:a3:v1 0x9f0:rt:a3:v1 0x9f8:rt:v1:t5 0x9fc:rt:a0:t7 0xa00:rt:a1:t9 0xa04:rt:a2:t3 0xa0c:rs:v1:t5 0xa0c:rt:t8:t6 0xa10:rs:a0:t7 0xa10:rt:t7:t8 0xa14:rs:a1:t9 0xa14:rt:t6:t1 0xa18:rs:a2:t3 0xa18:rt:t9:t4 0xa1c:rt:t6:t1 0xa20:rt:t7:t8 0xa24:rt:t8:t6 0xa28:rt:a3:v1 0xa2c:rt:t9:t4 0xa40:rt:t1:t2 0xa44:rs:t1:t2 0xa44:rt:t1:t2 0xa50:rt:a3:v1 0xa54:rs:t1:t2 0xa54:rt:t2:t3 0xa58:rt:t2:t3 0xa5c:rs:a3:v1 0xa5c:rt:a3:v1 0xa64:rt:v1:t6 0xa68:rt:a0:t8 0xa6c:rt:a1:t1 0xa70:rt:a2:t4 0xa78:rs:v1:t6 0xa78:rt:t5:t7 0xa7c:rs:a0:t8 0xa7c:rt:t4:t9 0xa80:rs:a1:t1 0xa80:rt:t3:t2 0xa84:rs:a2:t4 0xa84:rt:t6:t5 0xa88:rt:t3:t2 0xa8c:rt:t4:t9 0xa90:rt:t5:t7 0xa94:rt:a3:v1 0xa98:rt:t6:t5 */
#include "types.h"
#include "tokimeki/class_w.h"

extern u8  now_sub_sub_step;
extern u16 basyo_pack;
extern u16 bukatu_rec[];
extern s16 girl0_tokimeki;
extern u8  back_clear_b[];
extern u8  girl5_blk_end[];
extern u8  screen_w[];
extern s16 mes_base_y;
extern s16 now_serifu;

extern u8  cal_cw_prio;
extern u8  cal_cw_mode;
extern u8  cal_cw_disp;
extern u8  cal_cw_sel;
extern u8  cal_cw_rate;
extern u8  cal_cw_bright;
extern u16 cal_cw_tick;
extern s32 cal_cw_img;
extern s32 cal_cw_anim_tbl;
extern s16 cal_cw_aux14;
extern u16 cal_cw_anim;
extern u16 cal_cw_frame;
extern s16 cal_cw_x;
extern s16 cal_cw_y;
extern s32 cal_cw_hdr;
extern u32 cal_cw_attr;
extern u8  cal_cw_clut;

extern u8  master_bukatu_judge_str0[];
extern u8  master_bukatu_judge_str1[];
extern u8  master_bukatu_judge_str2[];
extern u32 master_bukatu_cw_ptr[];
extern u8  master_bukatu_hit_cnt;
extern u8  master_bukatu_rank;
extern u8  master_bukatu_hit_tbl[];
extern u8  master_bukatu_girl_no;
extern s16 master_bukatu_serifu_no;
extern CLASS_W class_w[];

extern void sub_step_up(void);
extern void sub_sub_step_up(void);
extern void goto_sub_step(s32);
extern void palette_upload_enqueue(s32, s32, s32, s32, s32);
extern void device_cmd_send(s32);
extern s32  get_CD_State2(void);
extern void cd_read_start2(s32, u32, u32);
extern void game_reset(void);
extern void class_clear_n(s32);
extern void k_reset(s32);
extern void indexed_byte_lsb_set(s32, s32);
extern s32  dec_bg_init(s32);
extern void value_cache_find_or_load(s32, s32);
extern void basyo_init(u8 *);
extern void fade_in_step(void);
extern void fade_out_step(void);
extern void serifu_run(void);
extern void common_disp(void);
extern s32  rand(void);

extern void master_bukatu_basyo10_ptr_set(void);
extern void master_bukatu_basyo7_ptr_set(void);
extern void master_bukatu_basyo9_ptr_set(void);
extern void master_bukatu_basyo8_ptr_set(void);

#define TIMER_BUMP(d)                                                   \
    girl0_tokimeki += (d);                                                  \
    tp = back_clear_b;                                                    \
    do {                                                                \
        ta = *(s16 *)(tp + 502);                                        \
        tb = *(s16 *)(tp + 558);                                        \
        tc = *(s16 *)(tp + 614);                                        \
        te = *(s16 *)(tp + 446);                                        \
        *(s16 *)(tp + 614) = tc + (d);                                  \
        *(s16 *)(tp + 558) = tb + (d);                                  \
        *(s16 *)(tp + 502) = ta + (d);                                  \
        *(s16 *)(tp + 446) = te + (d);                                  \
        tp += 224;                                                      \
    } while (tp != girl5_blk_end)

void master_bukatu_judge(void)
{
    u32 sc;
    s32 i;
    u32 c7;
    u32 sub;
    u32 b1;
    u32 b2;
    s32 j;
    s32 k;
    s32 nib;
    s16 cam;
    s16 ta;
    s16 tb;
    s16 tc;
    s16 te;
    u8 idx8;
    u8 *p;
    u8 *q;
    u8 *tp;

    switch (now_sub_sub_step) {
    case 0:
        switch ((u32)basyo_pack >> 12) {
        case 7:
            value_cache_find_or_load(16771, 1);
            break;
        case 8:
            value_cache_find_or_load(16798, 1);
            break;
        case 9:
            value_cache_find_or_load(16789, 1);
            break;
        case 10:
            value_cache_find_or_load(16781, 1);
            break;
        default:
            game_reset();
            break;
        }
        k_reset(0);
        basyo_init(master_bukatu_judge_str0);
        mes_base_y = 192;
        sub_sub_step_up();
        break;
    case 1:
        if (get_CD_State2() & 1) {
            indexed_byte_lsb_set(1, dec_bg_init(1));
            switch ((u32)basyo_pack >> 12) {
            case 7:
                cd_read_start2(26, 0x80197000, 0xAFE7);
                break;
            case 8:
                cd_read_start2(27, 0x80197000, 0xB01C);
                break;
            case 9:
                cd_read_start2(27, 0x80197000, 0xB001);
                break;
            case 10:
                cd_read_start2(27, 0x80197000, 0xAFCC);
                break;
            default:
                game_reset();
                break;
            }
            sub_sub_step_up();
        }
        break;
    case 2:
        fade_in_step();
        break;
    case 3:
        now_serifu = 12;
        sub_sub_step_up();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            switch ((u32)basyo_pack >> 12) {
            case 7:
                master_bukatu_basyo7_ptr_set();
                break;
            case 8:
                master_bukatu_basyo8_ptr_set();
                break;
            case 9:
                master_bukatu_basyo9_ptr_set();
                break;
            case 10:
                master_bukatu_basyo10_ptr_set();
                break;
            default:
                game_reset();
                break;
            }
            i = 0;
            do {
                idx8 = i + 16;
                palette_upload_enqueue((*(s32 *)&master_bukatu_cw_ptr[4]), idx8, 1, 1, i);
                i += 1;
            } while (i != 5);
            sub_sub_step_up();
        }
        break;
    case 5:
    case 11:
    case 13:
        serifu_run();
        break;
    case 6:
        sc = (u32)basyo_pack >> 12;
        cam = bukatu_rec[sc * 2] - 350;
        if (cam < -30) {
            master_bukatu_rank = 0;
        } else if (cam < -20) {
            master_bukatu_rank = 1;
        } else if (cam < -10) {
            master_bukatu_rank = 1;
        } else if (cam < 0) {
            master_bukatu_rank = 2;
        } else if (cam >= 30) {
            master_bukatu_rank = 5;
        } else if (cam >= 20) {
            master_bukatu_rank = 4;
        } else if (cam >= 10) {
            master_bukatu_rank = 4;
        } else if (cam >= 0) {
            master_bukatu_rank = 3;
        }
        sub_sub_step_up();
        break;
    case 7:
        i = 0;
        do {
            master_bukatu_hit_tbl[i] = 0;
            i++;
        } while (i < 5);
        c7 = master_bukatu_rank;
        i = 0;
        if (c7 != 0) {
            do {
                i++;
                master_bukatu_hit_tbl[i - 1] = 1;
            } while (i < c7);
        }
        i = 0;
        nib = rand() & 0xF;
        if (master_bukatu_rank >= 3U) {
            q = (u8 *)0x80140000;
            b1 = master_bukatu_hit_tbl[0];
            b2 = master_bukatu_hit_tbl[4];
            q[-14452] = b1;
            q[-14456] = b2;
        }
        if (nib > 0) {
            do {
                j = rand() & 3;
                k = rand() & 3;
                b1 = master_bukatu_hit_tbl[j];
                b2 = master_bukatu_hit_tbl[k];
                master_bukatu_hit_tbl[j] = b2;
                master_bukatu_hit_tbl[k] = b1;
                i++;
            } while (i != nib);
        }
        if (((u32)basyo_pack >> 12) == 9) {
            if (master_bukatu_rank >= 3U) {
                master_bukatu_hit_tbl[0] = 1;
            } else {
                master_bukatu_hit_tbl[0] = 0;
            }
        }
        sub_sub_step_up();
        break;
    case 8:
        class_clear_n(96);
        cal_cw_prio = 3;
        cal_cw_attr = 0x01000000;
        cal_cw_sel = 128;
        class_w[96].scale_x = 4096;
        class_w[96].scale_y = 4096;
        cal_cw_rate = 1;
        cal_cw_bright = 128;
        cal_cw_clut = rand() % 5 + 16;
        cal_cw_x = -96;
        cal_cw_y = 32;
        cal_cw_img = (*(s32 *)&master_bukatu_cw_ptr[1]);
        cal_cw_anim_tbl = (*(s32 *)&master_bukatu_cw_ptr[2]);
        cal_cw_hdr = (*(s32 *)&master_bukatu_cw_ptr[0]);
        cal_cw_aux14 = (*(s16 *)&master_bukatu_cw_ptr[3]);
        master_bukatu_hit_cnt = 0;
        sub_sub_step_up();
        break;
    case 9:
        if (cal_cw_mode == 0) {
            if ((sub = master_bukatu_hit_cnt) >= 5 ||
                (sub != 0 && ((u32)basyo_pack >> 12) == 9)) {
                sub_sub_step_up();
            } else {
                cal_cw_disp = 132;
                cal_cw_mode = 5;
                cal_cw_frame = 0;
                cal_cw_tick = 0;
                if (master_bukatu_hit_tbl[sub] == 1U) {
                    cal_cw_anim = 0;
                } else {
                    cal_cw_anim = 1;
                }
                master_bukatu_hit_cnt = master_bukatu_hit_cnt + 1;
            }
        }
        break;
    case 10:
        if (master_bukatu_rank >= 3U) {
            now_serifu = 13;
            device_cmd_send(516);
        } else {
            now_serifu = master_bukatu_serifu_no + 16;
            device_cmd_send(517);
        }
        sub_sub_step_up();
        break;
    case 12:
        switch ((u32)basyo_pack >> 12) {
        case 7:
            if (master_bukatu_rank >= 3U) {
                now_serifu = 23;
            } else {
                now_serifu = 102;
            }
            break;
        case 8:
            if (master_bukatu_rank >= 3U) {
                now_serifu = 24;
            } else {
                now_serifu = 103;
            }
            break;
        case 9:
            if (master_bukatu_rank >= 3U) {
                now_serifu = 25;
            } else {
                now_serifu = 104;
            }
            break;
        case 10:
            if (master_bukatu_rank >= 3U) {
                now_serifu = 26;
            } else {
                now_serifu = 105;
            }
            break;
        default:
            game_reset();
            break;
        }
        sub_sub_step_up();
        break;
    case 14:
        if (cal_cw_bright < 4U) {
            cal_cw_bright = 0;
        } else {
            cal_cw_bright = cal_cw_bright - 4;
        }
        fade_out_step();
        break;
    case 15:
        class_clear_n(96);
        if (master_bukatu_girl_no == 255) {
            if (master_bukatu_rank >= 3U) {
                TIMER_BUMP(25);
            } else {
                TIMER_BUMP(18);
            }
            goto_sub_step(6);
        } else {
            k_reset(0);
            if (((u32)basyo_pack >> 12) == 7) {
                basyo_init(master_bukatu_judge_str1);
            } else {
                basyo_init(master_bukatu_judge_str2);
            }
            mes_base_y = 192;
            *(s16 *)(screen_w + master_bukatu_girl_no * 56 + 450) += 2;
            if (master_bukatu_rank >= 3U) {
                TIMER_BUMP(25);
                sub_step_up();
            } else {
                TIMER_BUMP(18);
                goto_sub_step(5);
            }
        }
        break;
    default:
        game_reset();
        break;
    }
    common_disp();
}
