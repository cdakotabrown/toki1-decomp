/* @CompileEnv: IDO52_FORCE_WEB_ADJSAVE=292:1000 IDO52_FORCE_WEB_COLOR=292:s8,291:s6,0:s5 */
/* tt_scene_hit_step - TT overlay, Address: 0x801455B8 */
/* @SymbolSize: 2188   (547 instructions; the .sym size 2200 runs into 3 */
/* @PostProc: swap_instructions --offset1 0x3c8 --offset2 0x3e0 */
/* @PostProc: swap_instructions --offset1 0x530 --offset2 0x534 */
/* @PostProc: patch_insn_reg_field 0x180:rs:v0:s7 0x180:rt:s7:v0 0x188:rs:v0:fp 0x188:rt:fp:v0 0x25c:rt:v1:a1 0x2a0:rt:v1:a1 0x2ac:rs:v1:a1 0x2b4:rs:v1:s7 0x2b4:rt:s7:a1 0x374:rs:v0:s2 0x374:rt:s2:v0 0x394:rs:v0:s3 0x394:rt:s3:v0 0x3c8:rt:s2:s6 0x3e0:rt:s6:s2 0x4b4:rs:v0:s7 0x4b4:rt:s7:v0 0x4bc:rs:v0:fp 0x4bc:rt:fp:v0 0x4cc:rs:v0:s6 0x4cc:rt:s2:v0 0x500:rt:s6:s2 0x544:rt:t8:t3 0x554:rt:t8:t3 0x5bc:rt:v0:t3 0x5c0:rt:t3:t5 0x5c4:rs:v0:t3 0x5c4:rt:v0:t4 0x5c8:rs:t3:t5 0x5cc:rt:v0:t4 0x5d0:rs:v0:t4 0x5d0:rt:v1:v0 0x5d4:rs:v1:v0 0x5d4:rt:t4:t6 0x5d8:rs:t4:t6 0x5dc:rs:v1:v0 0x5dc:rt:t5:t7 0x5e0:rs:t5:t7 0x5f0:rt:t6:t8 0x5f4:rt:t6:t8 0x608:rt:t8:t0 0x614:rt:t7:t9 0x618:rt:t7:t9 0x624:rt:t8:t0 0x628:rt:t8:t0 0x67c:rt:t9:t1 0x684:rs:t9:t1 0x694:rs:v0:s7 0x694:rt:s7:v0 0x69c:rs:v0:fp 0x69c:rt:fp:v0 0x6a4:rt:v0:v1 0x6ac:rs:v0:v1 0x6b8:rs:v0:v1 0x6c0:rs:v0:v1 0x6c8:rs:v0:v1 0x718:rt:t0:t2 0x720:rs:t0:t2 0x720:rt:v0:a1 0x748:rt:v0:t3 0x74c:rt:t1:t5 0x750:rs:v0:t3 0x750:rt:v0:t4 0x754:rs:t1:t5 0x758:rt:v0:t4 0x75c:rs:v0:t4 0x75c:rt:t2:t7 0x760:rs:t2:t7 0x7b0:rt:t3:t8 0x7b8:rs:t3:t8 0x7b8:rt:v0:a1 0x7c4:rs:v0:a1 0x7cc:rs:v0:s7 0x7cc:rt:s7:a1 0x7d0:rt:t8:t3 0x7dc:rt:t4:t9 0x7e4:rs:t4:t9 0x7ec:rt:t5:t0 0x7f4:rs:t5:t0 0x818:rt:t6:t1 0x81c:rt:t7:t2 0x824:rs:t6:t1 0x824:rt:t7:t2 0x854:rt:t8:t3 0x85c:rs:t8:t3 0x85c:rt:t9:t4 0x860:rt:t9:t4 0x864:rs:t9:t4 */
#include "types.h"
#include "tokimeki/tt_box.h"

extern u8 *tt_player_wk;
extern u8 *tt_step_wk;
extern u8 *tt_slot_buf;
extern u8 *tt_shot_buf;
extern u8 *tt_bell_buf;
extern u8 *tt_obj_pool;
extern u8 *tt_trail_buf;
extern u8 *tt_player_shot_buf;

extern u8 tt_hit_box_tbl[];

extern s32 tt_box_overlap_check();
extern void tt_slot_hit_init();
extern void tt_widget_pair_init();
extern void tt_widget_step();
extern void tt_cmd_once();
extern void tt_entry_clear();
extern void tt_shot_triple_set();
extern void tt_shot_fan4_make();
extern void tt_shot_spread_make();
extern void tt_shot_aimed_make();
extern void tt_slot_hit_make();
extern void tt_sprite_hit_step();

void tt_scene_hit_step(void)
{
    u8 *ctr = tt_step_wk;
    u8 *savedW = tt_player_wk;
    u8 *w;
    u8 *we;
    u8 *wf;
    s32 j;
    s32 v;
    s32 v1;
    struct { s16 h0; s16 h2; u8 b4; u8 b5; } rc;
    s32 i;

    w = tt_slot_buf;
    for (i = 0; i < 64; i++, w += 120) {
        if (w[0] == 0) continue;
        if (*(u16 *)(w + 4) != 64) continue;
        if (*(u16 *)(w + 2) == 71 && *(u16 *)(w + 6) < 2U) continue;
        if (tt_box_overlap_check(savedW + 20, tt_hit_box_tbl, w + 20, ((u8 *)&((TT_BOX *)tt_hit_box_tbl)[2].x)) == 0)
            continue;
        if (*(u16 *)(savedW + 4) < 96U) {
            tt_slot_hit_init(w);
            *(s16 *)(w + 4) = 113;
            *(s16 *)(w + 10) = 0;
            *(s16 *)(w + 8) = 0;
            *(s16 *)(w + 6) = 0;
            w[15] = 0;
            w[14] = 0;
            w[13] = 0;
        }
        if (savedW[82] != 0) continue;
        tt_widget_pair_init();
        *(s16 *)(savedW + 4) = 96;
        break;
    }

    w = tt_shot_buf;
    for (i = 0; i < 128; i++, w += 104) {
        if (w[0] == 0) continue;
        v = *(u16 *)(w + 4);
        if (v != 64 && v != 80) continue;
        if (*(u16 *)(w + 2) == 173) {
            switch (v) {
            case 64:
                rc.h0 = 0;
                rc.h2 = (s16)((u32)*(u16 *)(w + 98) >> 1);
                rc.b4 = 2;
                rc.b5 = (u8)((u32)*(u16 *)(w + 98) >> 1);
                break;
            case 80:
                rc.h0 = 0;
                rc.h2 = 16;
                rc.b4 = 2;
                rc.b5 = 16;
                break;
            }
            *(u8 **)(w + 76) = (u8 *)&rc;
        }
        if (tt_box_overlap_check(savedW + 20, tt_hit_box_tbl, w + 20,
                          *(s32 *)(w + 76)) == 0)
            continue;
        if (*(u16 *)(savedW + 4) < 96U) {
            w[0] = 0;
        }
        if (savedW[82] != 0) continue;
        tt_widget_pair_init();
        *(s16 *)(savedW + 4) = 96;
        *(s16 *)(savedW + 10) = 0;
        *(s16 *)(savedW + 8) = 0;
        *(s16 *)(savedW + 6) = 0;
        savedW[15] = 0;
        savedW[14] = 0;
        savedW[13] = 0;
        break;
    }

    v = *(u16 *)(savedW + 4);
    if (v == 48 || v == 64) {
        w = tt_bell_buf;
        for (i = 0; i < 32; i++, w += 108) {
            if (w[0] == 0) continue;
            if (tt_box_overlap_check(savedW + 20, tt_hit_box_tbl, w + 20,
                              ((u8 *)&((TT_BOX *)tt_hit_box_tbl)[4].x)) == 0)
                continue;
            tt_widget_step(w);
        }
    }

    w = tt_obj_pool;
    for (i = 0; i < 32; i++, w += 120) {
        if (w[0] == 0) continue;
        if (*(u16 *)(w + 4) != 122) continue;
        if (tt_box_overlap_check(savedW + 20, tt_hit_box_tbl, w + 20, ((u8 *)&((TT_BOX *)tt_hit_box_tbl)[4].x)) == 0)
            continue;
        v = *(u16 *)(w + 2);
        if (v == 110) {
            tt_cmd_once(1551);
            w[0] = 0;
            tt_entry_clear();
            continue;
        }
        if (v == 111) {
            w[0] = 0;
            tt_shot_triple_set(w);
        }
    }

    if (savedW[83] != 0) {
        we = tt_trail_buf;
        for (i = 0; i < 3; i++, we += 164) {
            if (we[0] == 0) continue;

            w = tt_slot_buf;
            for (j = 0; j < 64; j++, w += 120) {
                if (w[0] == 0) continue;
                if (*(u16 *)(w + 4) != 64) continue;
                if (*(u16 *)(w + 2) == 71 && *(u16 *)(w + 6) < 2U) continue;
                if (tt_box_overlap_check(we + 20, ((u8 *)&((TT_BOX *)tt_hit_box_tbl)[1].x), w + 20,
                                  ((u8 *)&((TT_BOX *)tt_hit_box_tbl)[2].x)) == 0)
                    continue;
                tt_slot_hit_init(w);
                *(s16 *)(w + 4) = 113;
                *(s16 *)(w + 10) = 0;
                *(s16 *)(w + 8) = 0;
                *(s16 *)(w + 6) = 0;
                w[15] = 0;
                w[14] = 0;
                w[13] = 0;
            }

            w = tt_shot_buf;
            for (j = 0; j < 128; j++, w += 104) {
                if (w[0] == 0) continue;
                v = *(u16 *)(w + 4);
                if (v != 64 && v != 80) continue;
                v = *(u16 *)(w + 2);
                if (v == 171) continue;
                if (v == 173) continue;
                if (tt_box_overlap_check(we + 20, tt_hit_box_tbl, w + 20,
                                  *(s32 *)(w + 76)) == 0)
                    continue;
                w[0] = 0;
            }
        }
    }

    wf = tt_player_shot_buf;
    for (i = 0; i < 12; i++, wf += 84) {
        if (wf[0] == 0) continue;
        if (*(u16 *)(wf + 4) != 64) continue;

        w = tt_slot_buf;
        for (j = 0; j < 64; j++, w += 120) {
            u32 nx;
            if (w[0] == 0) continue;
            if (*(u16 *)(w + 4) != 64) continue;
            if (*(u16 *)(w + 2) == 71 && *(u16 *)(w + 6) < 2U) continue;
            if (tt_box_overlap_check(wf + 20, ((u8 *)&((TT_BOX *)tt_hit_box_tbl)[1].x), w + 20, ((u8 *)&((TT_BOX *)tt_hit_box_tbl)[2].x)) == 0)
                continue;
            nx = ctr[37] + 1;
            ctr[37] = nx;
            if (ctr[36] != 0) {
                v = nx & 0xFF;
                if (v & 1) {
                    if (v & 2) {
                        tt_shot_fan4_make(*(s16 *)(w + 20), *(s16 *)(w + 22),
                                      256, 32, 160);
                    } else {
                        tt_shot_spread_make(*(s16 *)(w + 20), *(s16 *)(w + 22),
                                      256, 32, 160);
                    }
                }
            }
            *(s16 *)(w + 4) = 112;
            *(s16 *)(w + 10) = 0;
            *(s16 *)(w + 8) = 0;
            *(s16 *)(w + 6) = 0;
            w[15] = 0;
            w[14] = 0;
            w[13] = 0;
            *(s16 *)(wf + 4) = 128;
            wf[13] = 0;
            wf[14] = 0;
            wf[15] = 0;
            *(s16 *)(wf + 6) = 0;
            *(s16 *)(wf + 8) = 0;
            *(s16 *)(wf + 10) = 0;
        }

        w = tt_shot_buf;
        for (j = 0; j < 128; j++, w += 104) {
            if (w[0] == 0) continue;
            v = *(u16 *)(w + 4);
            if (v != 64 && v != 80) continue;
            v1 = *(u16 *)(w + 2);
            switch (v1) {
            case 162:
            case 169:
            case 170:
                if (tt_box_overlap_check(wf + 20, ((u8 *)&((TT_BOX *)tt_hit_box_tbl)[1].x), w + 20,
                                  *(s32 *)(w + 76)) != 0) {
                    tt_cmd_once(1289);
                    w[0] = 0;
                    *(s16 *)(wf + 4) = 128;
                    wf[13] = 0;
                    wf[14] = 0;
                    wf[15] = 0;
                    *(s16 *)(wf + 6) = 0;
                    *(s16 *)(wf + 8) = 0;
                    *(s16 *)(wf + 10) = 0;
                    goto inner8;
                }
                break;
            case 172:
                {
                u32 nx;
                if (tt_box_overlap_check(wf + 20, ((u8 *)&((TT_BOX *)tt_hit_box_tbl)[1].x), w + 20,
                                  *(s32 *)(w + 76)) == 0)
                    continue;
                nx = ctr[37] + 1;
                ctr[37] = nx;
                if (ctr[36] != 0) {
                    if (nx & 1) {
                        tt_shot_aimed_make(*(s16 *)(w + 20), *(s16 *)(w + 22),
                                      32, 160);
                    }
                }
                w[0] = 0;
                tt_slot_hit_make(w + 20);
                *(s16 *)(wf + 4) = 128;
                *(s16 *)(wf + 10) = 0;
                *(s16 *)(wf + 8) = 0;
                *(s16 *)(wf + 6) = 0;
                wf[15] = 0;
                wf[14] = 0;
                wf[13] = 0;
                }
                break;
            }
        }

    inner8:

        v = *(u16 *)(savedW + 4);
        if (v == 48 || v == 64) {
            w = tt_bell_buf;
            for (j = 0; j < 32; j++, w += 108) {
                if (w[0] == 0) continue;
                if (*(u16 *)(w + 4) >= 144U) continue;
                if (tt_box_overlap_check(wf + 20, ((u8 *)&((TT_BOX *)tt_hit_box_tbl)[1].x), w + 20,
                                  ((u8 *)&((TT_BOX *)tt_hit_box_tbl)[4].x)) == 0)
                    continue;
                tt_sprite_hit_step(w, *(s16 *)(wf + 20) - *(s16 *)(w + 20));
                *(s16 *)(wf + 4) = 128;
                *(s16 *)(wf + 10) = 0;
                *(s16 *)(wf + 8) = 0;
                *(s16 *)(wf + 6) = 0;
                wf[15] = 0;
                wf[14] = 0;
                wf[13] = 0;
            }
        }
    }
}
