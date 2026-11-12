/* @PostProc: patch_stack_complete --old 144 --new 168 */
/* @PostProc: remap_stack_slots --map '124:96,136:156,112:116,114:118' */
/* @PostProc: scheduler_normalizer --block-aware */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x1c */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x20 */
/* @PostProc: swap_instructions --offset1 0x138 --offset2 0x108 */
/* @PostProc: swap_instructions --offset1 0x12c --offset2 0x110 */
/* @PostProc: swap_instructions --offset1 0x138 --offset2 0x12c */
/* @PostProc: swap_instructions --offset1 0x1c0 --offset2 0x1a4 */
/* @PostProc: swap_instructions --offset1 0x1c0 --offset2 0x1b0 */
/* @PostProc: swap_instructions --offset1 0x1c8 --offset2 0x1c0 */
/* @PostProc: patch_insn_reg_field 0x14:rt:t6:t7 0x18:rs:t6:t7 0x18:rt:t6:t7 0x1c:rt:t7:t6 0x20:rs:t7:t6 0x20:rt:t7:t6 0x4c:rt:t6:t7 0x50:rs:t7:t6 0x54:rt:s5:s4 0x58:rs:s5:s4 0x58:rt:s5:s4 0x60:rs:t7:t6 0x64:rd:s4:s6 0x68:rd:v0:s1 0x68:rs:t7:t6 0x6c:rs:s4:s6 0x6c:rt:s4:s6 0x70:rs:s4:s6 0x74:rs:v0:s1 0x74:rt:v0:s1 0x7c:rs:v0:s1 0xac:rd:s4:s6 0xc4:rt:s6:s5 0xc8:rt:t2:t1 0xcc:rt:t1:t3 0xd0:rs:t2:t1 0xd0:rt:t3:t2 0xd8:rs:t1:t2 0xe0:rt:t5:t7 0xe4:rs:t2:t1 0xe4:rt:t6:t5 0xec:rd:t7:t6 0xec:rt:t6:t7 0xf0:rt:t7:t6 0x114:rd:t4:t2 0x114:rt:t3:s6 0x118:rs:t4:t2 0x118:rt:s2:s6 0x11c:rd:t5:t2 0x11c:rt:v0:t2 0x120:rd:a3:t3 0x120:rs:t7:s2 0x120:rt:fp:t2 0x124:rs:t2:t3 0x124:rt:v0:t4 0x12c:rt:s4:t4 0x130:rd:t6:t1 0x130:rs:t5:t1 0x130:rt:v0:t4 0x134:rd:t7:t1 0x134:rt:t6:t1 0x138:rd:t3:a3 0x138:rs:t1:fp 0x138:rt:s4:t1 0x158:rt:t0:t7 0x15c:rt:t9:t5 0x160:rs:t0:t7 0x160:rt:t9:t5 0x164:rs:t0:t7 0x168:rs:t0:t7 0x16c:rs:t0:t7 0x170:rs:t0:t7 0x174:rs:t0:t7 0x178:rs:t0:t7 0x188:rt:t1:t6 0x190:rs:t1:t6 0x198:rt:t3:t9 0x1a0:rs:t3:t9 0x1a8:rd:t5:t0 0x1a8:rt:v0:s6 0x1ac:rd:t2:t0 0x1ac:rs:t4:t0 0x1ac:rt:s4:s6 0x1b0:rd:t4:t0 0x1b0:rt:s4:t0 0x1b4:rs:t8:s2 0x1b4:rt:s2:t0 0x1b8:rt:v0:t8 0x1c0:rd:t8:t2 0x1c0:rt:t2:t8 0x1c4:rd:t6:t2 0x1c4:rs:t5:t2 0x1c4:rt:v0:t8 0x1c8:rd:t7:t2 0x1c8:rt:t6:t2 0x1cc:rs:t7:fp 0x1cc:rt:fp:t2 0x1e8:rs:s5:s4 0x1e8:rt:v1:t3 0x1ec:rs:s5:s4 0x1ec:rt:v0:t1 0x1f0:rs:v1:t3 0x1f0:rt:v1:t4 0x1f4:rs:v0:t1 0x1f8:rs:s5:s4 0x1f8:rt:v1:t4 0x1fc:rs:v1:t4 0x1fc:rt:t9:t7 0x200:rs:t9:t7 0x218:rt:t0:t6 0x21c:rt:t0:t6 0x238:rt:t1:t9 0x240:rs:t1:t9 0x248:rt:t3:t0 0x250:rs:t3:t0 0x258:rt:v0:t8 0x260:rs:v0:t8 0x260:rt:v0:t3 0x264:rs:v0:t3 0x26c:rt:v0:t3 0x270:rt:t2:t1 0x27c:rt:t5:t7 0x284:rd:t8:t5 0x284:rt:t2:t1 0x288:rs:t5:t7 0x288:rt:t8:t5 0x2ac:rt:t7:t9 0x2b0:rt:t7:t9 0x2c8:rt:s6:s5 0x2cc:rd:t0:t8 0x2d0:rd:t1:t2 0x2d0:rs:t7:t9 0x2d0:rt:t0:t8 0x2d4:rs:t1:t2 0x304:rt:s6:s5 0x310:rd:t5:t7 0x314:rd:t8:t5 0x314:rt:t5:t7 0x318:rs:t8:t5 0x330:rt:t7:t9 0x334:rs:s4:s6 0x334:rt:s4:s6 0x33c:rs:t7:t9 0x33c:rt:t9:t0 0x340:rs:s4:s6 0x344:rt:t9:t0 */
/* tt_hit_scan - TT overlay, Address: 0x80135360, Size: 872 bytes (218 insns) */
#include "types.h"
#include "tokimeki/tt_box.h"

extern u8 tt_hit_box_tbl[];

extern u8 *tt_step_wk;
extern u8 *tt_player_wk;
extern u8 *tt_player_shot_buf;
extern u8 *tt_boss_wk;

extern s32 tt_hit_check(void);
extern void tt_cmd_once(s32 soundId);
extern s32 tt_box_overlap_check(u8 *a0, u8 *a1, s16 *a2, u8 *a3);
extern void tt_widget_pair_init(void);
extern void tt_shot_aimed_make(s16 x, s16 y, s32 w, s32 h);
extern void tt_shot_heading_make(s16 x, s16 y, s32 a2, s32 a3, s32 a4);

void tt_hit_scan(void)
{
    u8 *base;
    u8 *cur;
    u8 *hit;
    u8 *obj;
    u8 *wp;
    u8 *p;
    u8 *tbl;
    s16 pos[2];
    s32 i;
    s32 j;
    u32 c;
    u32 v;
    u32 cnt;
    u32 cc;
    u32 prev;
    u16 kk;

    cur = tt_player_wk;
    hit = tt_step_wk;
    base = tt_boss_wk;
    if (base[0] == 0) {
        return;
    }
    for (i = 0; i < 24; i++) {
        base[(i * 20) + 66] = 0;
    }

    if (base[834] == 0) {
        if (tt_hit_check() != 0) {
            tt_cmd_once(1283);
        }
    }
    if (base[836] != 0) {
        return;
    }
    tbl = ((u8 *)&((TT_BOX *)tt_hit_box_tbl)[6].x);
    wp = base;
    i = 0;
    do {
        pos[0] = *(s16 *)(wp + 600) + *(s16 *)(base + 20);
        pos[1] = *(s16 *)(wp + 648) + *(s16 *)(base + 22);
        if (cur[82] == 0 && cur[0] != 0) {
            c = *(base + (((i << 2) + i) << 2) + 222);
            if (tt_box_overlap_check(cur + 20, tt_hit_box_tbl, pos,
                              tbl + (((c << 2) - c) << 1) + 66) != 0) {
                tt_widget_pair_init();
                *(s16 *)(cur + 4) = 96;
                *(s16 *)(cur + 10) = 0;
                *(s16 *)(cur + 8) = 0;
                *(s16 *)(cur + 6) = 0;
                cur[15] = 0;
                cur[14] = 0;
                cur[13] = 0;
            }
        }
        obj = tt_player_shot_buf;
        j = 0;
        do {
            if (obj[0] != 0 && *(u16 *)(obj + 4) == 64) {
                p = base + (((i << 2) + i) << 2);
                c = p[222];
                if (tt_box_overlap_check(obj + 20, ((u8 *)&((TT_BOX *)tt_hit_box_tbl)[1].x), pos,
                                  tbl + (((c << 2) - c) << 1) + 66) != 0) {
                    cnt = hit[37] + 1;
                    prev = hit[36];
                    hit[37] = cnt;
                    if (prev != 0 && (cnt & 3) == 0) {
                        tt_shot_aimed_make(*(s16 *)(obj + 20), *(s16 *)(obj + 22),
                                      32, 160);
                    }
                    *(s16 *)(obj + 4) = 128;
                    *(s16 *)(obj + 10) = 0;
                    *(s16 *)(obj + 8) = 0;
                    *(s16 *)(obj + 6) = 0;
                    obj[15] = 0;
                    obj[14] = 0;
                    obj[13] = 0;
                    if (base[835] != 0) {
                        if (*(u16 *)(base + 6) == 1) {
                            cc = *(u16 *)(base + 952) + 1;
                            *(u16 *)(base + 952) = cc;
                            if ((cc & 7) == 7) {
                                tt_shot_heading_make(pos[0], pos[1], 24,
                                              3072 - (p[222] << 7), 160);
                            }
                        }
                        v = p[222];
                        if (v != 0 && v < 16U) {
                            kk = 0xC000;
                            *(s16 *)(base + 938) = kk;
                            v = p[222];
                            if (v >= 9U) {
                                *(s16 *)(base + 938) =
                                    (*(u16 *)(base + 938) + (v * 1843)) -
                                    14744;
                            }
                        } else if (v >= 17U) {
                            *(s16 *)(base + 938) = 16384;
                            v = p[222];
                            if (v < 24U) {
                                *(s16 *)(base + 938) =
                                    (*(u16 *)(base + 938) + (v * 1843)) -
                                    44232;
                            }
                        }
                    }
                }
            }
            j += 1;
            obj += 84;
        } while (j != 12);
        i += 1;
        wp += 2;
    } while (i != 8);
}
