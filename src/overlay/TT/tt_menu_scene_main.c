/* @RodataBase: 0x8014D7E4 */
/* @SymbolSize: 1700 */
/* @PostProc: patch_stack_complete --old 72 --new 88 */
/* @PostProc: remap_stack_slots --map '32:48,56:68' */
/* @PostProc: patch_insn_reg_field 0x24:rt:a0:t7 0x38:rt:a3:a0 0x48:rs:a3:a0 0x48:rt:a3:a0 0x50:rs:a0:t7 0x5c:rt:a0:t7 0xf4:rt:t0:a3 0xfc:rt:t0:a3 0x184:rt:t1:t0 0x188:rs:t1:t0 0x188:rt:t1:t0 0x18c:rt:t2:t1 0x198:rt:t1:t0 0x1a0:rt:t2:t1 0x1b8:rt:t5:t4 0x1c4:rt:t3:t2 0x1cc:rs:t3:t2 0x1cc:rt:t4:t3 0x1d0:rt:t4:t3 0x1d8:rt:t5:t4 0x1dc:rt:t6:t5 0x1e0:rs:t6:t5 0x1e0:rt:t6:t5 0x1e4:rt:t4:t2 0x1e8:rs:t6:t5 0x1e8:rt:t7:t6 0x1f0:rs:t7:t6 0x208:rt:t8:v1 0x210:rd:t9:t7 0x210:rs:t8:v1 0x214:rs:t9:t7 0x214:rt:t0:t8 0x218:rs:t0:t8 0x218:rt:t1:t9 0x21c:rs:t1:t9 0x230:rt:t2:t0 0x238:rs:t2:t0 0x238:rt:t3:t1 0x240:rt:t3:t1 0x244:rt:t4:t2 0x25c:rt:t3:t1 0x260:rt:t5:t3 0x264:rs:t5:t3 0x264:rt:t5:t3 0x26c:rs:t5:t3 0x26c:rt:t6:t4 0x274:rs:t6:t4 0x28c:rt:t7:t5 0x294:rs:t7:t5 0x294:rt:t8:t6 0x298:rt:t8:t6 0x2b0:rt:t3:t1 0x2b4:rt:t9:t7 0x2b8:rs:t9:t7 0x2b8:rt:t9:t7 0x2bc:rt:a2:a3 0x2c0:rs:t9:t7 0x2c0:rt:t0:t8 0x2c4:rt:t1:t9 0x2c8:rs:t0:t8 0x2d0:rt:t2:t0 0x2d8:rt:a2:a3 0x2dc:rs:t2:t0 0x2dc:rt:t2:t0 0x2e0:rt:t1:t9 0x2e4:rt:t2:t0 0x2e8:rt:t3:t1 0x2ec:rt:t3:t1 0x320:rt:t4:t2 0x328:rs:t4:t2 0x328:rt:t5:t3 0x330:rt:t5:t3 0x334:rt:v0:t4 0x338:rt:t8:t6 0x33c:rd:t9:t5 0x33c:rt:v0:t4 0x340:rs:t8:t6 0x340:rt:t8:t6 0x344:rt:t6:t7 0x348:rd:t9:t5 0x348:rs:t9:t5 0x348:rt:v0:t4 0x34c:rd:t9:t5 0x34c:rt:t9:t5 0x350:rd:t0:v0 0x350:rs:t8:t5 0x350:rt:t9:t6 0x354:rs:t6:t7 0x354:rt:t7:t8 0x358:rs:t0:v0 0x358:rt:t7:t8 0x35c:rt:t1:t9 0x364:rs:t1:t9 0x378:rt:t3:t1 0x380:rt:t2:t0 0x38c:rt:t3:t1 0x394:rt:t2:t0 0x3a0:rt:t3:t1 0x3a8:rt:t3:t1 0x3b8:rt:t3:t1 0x3bc:rs:a3:a0 0x3c0:rt:t4:t6 0x3c4:rt:t4:t6 0x3c8:rt:t6:t8 0x3cc:rt:t5:t7 0x3d0:rt:a2:a3 0x3d4:rt:t5:t7 0x3d8:rt:t1:t9 0x3dc:rt:a0:a1 0x3e0:rt:a1:a2 0x3e4:rt:t6:t8 0x3e8:rt:t2:t0 0x3ec:rt:t8:t2 0x3f0:rt:t9:t3 0x3f4:rt:t7:t4 0x3f8:rt:t0:t5 0x400:rt:t8:t2 0x404:rt:a2:a3 0x410:rt:a0:a1 0x418:rt:a2:a3 0x420:rt:a0:a1 0x428:rt:a2:a3 0x42c:rt:a1:a2 0x430:rt:a1:a2 0x434:rt:t9:t3 0x438:rt:t7:t4 0x440:rt:t0:t5 0x44c:rt:t1:t9 0x450:rt:t2:t0 0x454:rt:t3:t1 0x458:rt:t4:t2 0x45c:rt:t4:t2 0x460:rt:t5:t3 0x464:rt:t3:t1 0x468:rt:t5:t3 0x478:rt:t6:t4 0x480:rs:t6:t4 0x480:rt:t8:t5 0x488:rt:t8:t5 0x498:rt:t3:t1 0x49c:rt:t9:t6 0x4a0:rs:t9:t6 0x4a0:rt:t9:t6 0x4a8:rs:t9:t6 0x4a8:rt:v0:v1 0x4ac:rt:a2:a3 0x4b0:rs:v0:v1 0x4b8:rt:a2:a3 0x4bc:rs:v0:v1 0x4bc:rt:a2:a3 0x4c0:rt:t0:t8 0x4c8:rs:v0:v1 0x4cc:rt:t1:t9 0x4d4:rs:v0:v1 0x4d8:rt:t2:t0 0x4e0:rt:a2:a3 0x4f0:rt:t0:t8 0x4f8:rt:t1:t9 0x4fc:rt:t2:t0 0x500:rt:a2:a3 0x504:rt:t3:t1 0x50c:rs:t3:t1 0x53c:rt:v0:v1 0x540:rs:v0:v1 0x540:rt:v0:v1 0x544:rd:v1:v0 0x54c:rs:v1:v0 0x54c:rt:v1:v0 0x550:rs:v0:v1 0x554:rs:v0:v1 0x558:rs:v0:v1 0x55c:rs:v0:v1 0x55c:rt:v0:v1 0x560:rs:v1:v0 0x564:rs:v0:v1 0x570:rt:v0:v1 0x574:rs:v0:v1 0x574:rt:v0:v1 0x578:rd:v1:v0 0x57c:rt:t0:t2 0x580:rt:a3:t1 0x584:rt:a2:t0 0x588:rt:a1:a3 0x58c:rt:a0:a2 0x590:rs:v1:v0 0x590:rt:v1:v0 0x594:rs:v0:v1 0x598:rs:v0:v1 0x598:rt:a0:a2 0x59c:rs:v0:v1 0x59c:rt:a1:a3 0x5a0:rs:v0:v1 0x5a0:rt:a2:t0 0x5a4:rs:v0:v1 0x5a4:rt:a3:t1 0x5a8:rs:v0:v1 0x5ac:rs:v0:v1 0x5b0:rs:v0:v1 0x5b4:rs:v0:v1 0x5b8:rs:v0:v1 0x5bc:rs:v0:v1 0x5c0:rs:v0:v1 0x5c4:rs:v0:v1 0x5c4:rt:a0:a2 0x5c8:rs:v0:v1 0x5c8:rt:a1:a3 0x5cc:rs:v0:v1 0x5cc:rt:a2:t0 0x5d0:rs:v0:v1 0x5d0:rt:a3:t1 0x5d4:rs:v0:v1 0x5d8:rs:v0:v1 0x5dc:rs:v0:v1 0x5e0:rs:v0:v1 0x5e4:rs:v0:v1 0x5e8:rs:v0:v1 0x5ec:rs:v0:v1 0x5f0:rs:v0:v1 0x5f0:rt:a0:a2 0x5f4:rs:v0:v1 0x5f4:rt:a1:a3 0x5f8:rs:v0:v1 0x5f8:rt:a2:t0 0x5fc:rs:v0:v1 0x5fc:rt:a3:t1 0x600:rs:v0:v1 0x604:rs:v0:v1 0x608:rs:v0:v1 0x60c:rs:v0:v1 0x610:rs:v0:v1 0x614:rs:v0:v1 0x618:rs:v0:v1 0x618:rt:v0:v1 0x61c:rs:v0:v1 0x620:rs:v0:v1 0x620:rt:a0:a2 0x624:rs:v0:v1 0x624:rt:a1:a3 0x628:rs:v0:v1 0x628:rt:a2:t0 0x62c:rs:v0:v1 0x62c:rt:a3:t1 0x630:rs:v0:v1 0x634:rs:v0:v1 0x638:rs:v0:v1 0x63c:rs:v0:v1 0x640:rs:v0:v1 0x644:rs:v1:v0 0x644:rt:t0:t2 0x648:rs:v0:v1 0x654:rt:v0:t6 0x65c:rs:v0:t6 0x660:rt:v0:t6 0x668:rs:v0:t6 0x66c:rt:t8:t9 0x674:rs:v0:t6 0x678:rt:t0:t1 0x67c:rt:t6:t7 0x680:rt:t7:t8 0x684:rt:t6:t7 0x68c:rt:t7:t8 0x690:rt:t9:t0 0x694:rt:t8:t9 0x69c:rt:t9:t0 0x6a0:rt:t1:t2 0x6a4:rt:t0:t1 0x6a8:rt:t1:t2 */
/* tt_menu_scene_main - TT overlay, Address: 0x8014A2A4 */
#include "types.h"

extern u8 *tt_game_wk;
extern u8 *tt_step_wk;
extern u8 *tt_scroll_wk;
extern u8 *tt_player_wk;
extern u8 *tt_player_shot_buf;
extern u8 *tt_anim_buf;
extern u8 *tt_text_buf;
extern u8 *tt_scale_buf;

extern u8 tt_replay0_dat[];
extern u8 tt_main1_seq_dat[];

extern void device_cmd_send(s32);
extern s32 device_ready_check(s32);
extern s32 get_reads(void);
extern void tt_obj_clear(void);
extern void tt_cmd_once(s32);
extern void tt_shot_clear(void);
extern void tt_sprite_pool_reset(void);
extern void tt_slot_clear(void);
extern void tt_cd_read(u32, u32, u32);
extern s32 get_tt_read_prev(void);
extern void tt_active_clear(void);
extern void tt_rec_clear(void);
extern void tt_ent_clear(void);
extern void trail_clear_all(void);
extern void tt_text_clear(void);
extern void tt_recs_clear(void);
extern void tt_menu_cursor(void);
extern void tt_menu_init(void);
extern void tt_hud_init(void);
extern void tt_camera_swing(void);
extern void tt_menu_player_init(void);
extern void tt_menu_player_move(void);
extern void tt_menu_start_step(void);
extern void tt_sprite_anim(void);
extern void tt_ball_spin(void);
extern void tt_pool_clear(void);
extern void tt_record_clear(void);

void tt_player_shot_clear(void);

void tt_menu_scene_main(void)
{
    u8 *ctl;
    u8 *rec;
    u8 *pad;
    u8 *st;
    u8 *cur;
    u8 *cel;
    u32 step;
    u32 idx;
    u32 v;
    s32 a;
    s32 b;

    ctl = tt_step_wk;
    step = *(u16 *)(ctl + 2);
    rec = tt_anim_buf + 3200;
    pad = tt_game_wk;
    st = tt_scroll_wk;
    cur = tt_player_wk;
    cel = tt_scale_buf;

    switch (step) {
    case 0:
        if (*(u16 *)(tt_game_wk + 164) != 0) {
            tt_cd_read(0x80016000, 51232, 9248);
            tt_cd_read(0x8002E800, 51151, 27108);
            tt_cd_read(0x8019A000, 51155, 163168);
            tt_cd_read(0x8001A000, 49119, 3616);
            tt_cd_read(0x801C2000, 49066, 108160);
        }
        *(s16 *)(ctl + 2) = 1;
        break;
    case 1:
        device_cmd_send(34);
        device_cmd_send(47);
        device_cmd_send(517);
        tt_record_clear();
        tt_text_clear();
        tt_active_clear();
        tt_player_shot_clear();
        tt_rec_clear();
        tt_recs_clear();
        trail_clear_all();
        tt_pool_clear();
        tt_obj_clear();
        tt_shot_clear();
        tt_sprite_pool_reset();
        tt_ent_clear();
        tt_slot_clear();
        ctl[17] = 0;
        ctl[36] = 0;
        *(s32 *)(ctl + 24) = (s32)tt_main1_seq_dat;
        ctl[28] = 32;
        tt_menu_init();
        tt_hud_init();
        tt_menu_player_init();
        *(s16 *)(st + 16) = 0;
        st[10] = 0;
        *(s16 *)(ctl + 38) = 0;
        *(s16 *)(ctl + 2) = *(u16 *)(ctl + 2) + 1;
        *(s16 *)(ctl + 4) = 64;
        break;
    case 2:
        if (*(u16 *)(tt_game_wk + 164) != 0) {
            a = get_reads();
            b = get_tt_read_prev();
            if ((((a - b) + 256) & 0xFF) >= 5) {
                device_cmd_send(18);
                *(s16 *)(ctl + 2) = *(u16 *)(ctl + 2) + 1;
            }
        } else {
            *(s16 *)(ctl + 2) = 4;
        }
        tt_menu_player_move();
        tt_camera_swing();
        break;
    case 3:
        if (*(u16 *)(tt_game_wk + 164) != 0) {
            *(s16 *)(pad + 164) = 0;
            if (device_ready_check(1) != 0) {
                *(s16 *)(ctl + 2) = *(u16 *)(ctl + 2) + 1;
            }
        }
        tt_menu_player_move();
        tt_camera_swing();
        break;
    case 4:
        if (*(u16 *)(tt_step_wk + 38) >= 1800U) {
            ctl[36] = 0;
            ctl[34] = 1;
            pad[82] = 255;
            *(s32 *)(pad + 76) = (s32)tt_replay0_dat;
            *(s16 *)(ctl + 0) = 2;
            ctl[9] = 0;
            ctl[10] = 0;
            ctl[11] = 0;
            *(s16 *)(ctl + 2) = 0;
            *(s16 *)(ctl + 4) = 0;
            *(s16 *)(ctl + 6) = 0;
        }
        tt_menu_player_move();
        tt_camera_swing();
        tt_menu_cursor();
        *(s16 *)(ctl + 38) = *(u16 *)(ctl + 38) + 1;
        break;
    case 5:
        idx = st[10];
        tt_text_buf[idx * 20] = (u8)(*(u16 *)(pad + 8) & 1);
        if (st[10] == 0) {
            tt_menu_start_step();
            break;
        }
        v = *(u16 *)(st + 16);
        if (v >= 30U) {
            *(s16 *)(st + 16) = 0;
            *(s16 *)(ctl + 2) = 9;
            v = *(u16 *)(st + 16);
        }
        *(s16 *)(st + 16) = v + 1;
        break;
    case 6:
        tt_sprite_anim();
        break;
    case 7:
        cur[0] = 0;
        *(s32 *)(rec + 32) = 0x00100000;
        *(s32 *)(rec + 36) = 0x006C0000;
        *(s16 *)(rec + 20) = *(s16 *)(rec + 34);
        *(s16 *)(rec + 82) = 0;
        *(s16 *)(rec + 90) = 384;
        rec[0] = 1;
        rec[1] = 0;
        *(s16 *)(rec + 68) = 0;
        rec[59] = 128;
        rec[77] = 0;
        rec[78] = 1;
        rec[80] = 0;
        rec[70] = 128;
        rec[71] = 0;
        rec[79] = 1;
        *(s16 *)(rec + 60) = 128;
        *(s16 *)(rec + 62) = 128;
        *(s16 *)(rec + 64) = 26;
        *(s16 *)(rec + 66) = 15620;
        rec[76] = 0;
        *(s16 *)(rec + 16) = 2;
        *(s32 *)(rec + 40) = 0;
        *(s32 *)(rec + 44) = 0;
        *(s16 *)(rec + 22) = *(s16 *)(rec + 38);
        *(s16 *)(cel + 4) = *(s16 *)(rec + 20);
        *(s16 *)(cel + 6) = *(s16 *)(rec + 22);
        *(s16 *)(cel + 2) = 4096;
        *(s16 *)(cel + 0) = *(s16 *)(cel + 2);
        *(s32 *)(rec + 72) = (s32)cel;
        tt_cmd_once(1556);
        *(s16 *)(ctl + 2) = *(u16 *)(ctl + 2) + 1;
        break;
    case 8:
        tt_ball_spin();
        break;
    case 9:
        switch ((u32)tt_scroll_wk[10]) {
        case 0:
            *(s16 *)(ctl + 0) = 2;
            break;
        case 1:
            *(s16 *)(ctl + 0) = 7;
            break;
        case 2:
            *(s16 *)(ctl + 0) = 3;
            break;
        case 3:
            *(s16 *)(ctl + 0) = 8;
            break;
        }
        ctl[9] = 1;
        break;
    }

    if (ctl[9] != 0) {
        *(s16 *)(ctl + 6) = 0;
        *(s16 *)(ctl + 4) = 0;
        *(s16 *)(ctl + 2) = 0;
        ctl[11] = 0;
        ctl[10] = 0;
        ctl[9] = 0;
    }
}

void tt_player_shot_clear(void)
{
    u8 *wf;
    s32 i;

    wf = tt_player_shot_buf;
    for (i = 0; i < 12; i++) {
        wf[0] = 0;
        wf += 84;
    }
}

void tt_player_shot_init(void)
{
    u8 *wf;
    s32 i;

    wf = tt_player_shot_buf;
    for (i = 0; i < 12; i++) {
        wf[0] = 0;
        *(s16 *)(wf + 16) = 3;
        *(s16 *)(wf + 62) = 10;
        *(s16 *)(wf + 64) = 15495;
        *(s16 *)(wf + 4) = 64;
        *(s16 *)(wf + 10) = 0;
        *(s16 *)(wf + 8) = 0;
        *(s16 *)(wf + 6) = 0;
        wf[15] = 0;
        wf[14] = 0;
        wf[13] = 0;
        wf += 84;
    }
}

void tt_sprite_uv_set(u8 *p, s32 arg)
{
    u16 v;

    v = (u16)arg;
    *(s16 *)(p + 2) = v;
    switch (v) {
    case 8:
    default:
        p[66] = 128;
        p[67] = 192;
        break;
    case 9:
        p[66] = 144;
        p[67] = 192;
        break;
    case 10:
        p[66] = 128;
        p[67] = 208;
        break;
    }
}
