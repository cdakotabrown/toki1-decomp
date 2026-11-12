/* @CompileEnv: IDO52_LDA_NOINREG_CLASSES=4 IDO52_CONST_NOINREG_VALUES=128,20 */
/* @PostProc: swap_instructions --offset1 0x20 --offset2 0x34 */
/* @PostProc: swap_instructions --offset1 0xc8 --offset2 0xcc */
/* @PostProc: patch_insn_reg_field 0xc8:rt:s7:fp 0xcc:rt:fp:s7 0x50:rt:s4:s3 0x58:rs:s4:s3 0x58:rt:s4:s3 0x60:rd:s3:s4 0xec:rs:s7:fp 0x10c:rs:s4:s3 0x10c:rt:v1:t2 0x110:rs:s4:s3 0x110:rt:v0:t4 0x114:rs:v1:t2 0x114:rt:v1:t3 0x118:rs:v0:t4 0x11c:rs:s4:s3 0x11c:rt:v1:t3 0x120:rs:v1:t3 0x120:rt:t2:t7 0x124:rs:t2:t7 0x144:rt:t3:t6 0x148:rt:t3:t6 0x160:rs:s3:s4 0x168:rt:s3:s4 0x178:rt:t4:t8 0x180:rs:t4:t8 0x180:rt:t5:t9 0x188:rt:t5:t9 0x18c:rt:t7:t0 0x194:rs:t7:t0 0x19c:rt:t6:t1 0x1a4:rs:t6:t1 0x1a4:rt:t8:t2 0x1ac:rt:t8:t2 0x1b0:rt:t9:t3 0x1b8:rs:t9:t3 0x1b8:rt:t0:t4 0x1bc:rt:t0:t4 0x1c0:rs:s3:s4 0x1d4:rd:t1:t5 0x1d8:rd:t1:t5 0x1d8:rs:t1:t5 0x1dc:rd:t1:t5 0x1dc:rt:t1:t5 0x1e0:rd:t2:t7 0x1e0:rt:t1:t5 0x1e4:rs:t2:t7 0x1e8:rt:t3:t6 0x1f0:rs:t3:t6 0x1fc:rt:s3:s4 0x208:rt:fp:s7 */
/* tt_cell_hit_scan - TT overlay, Address: 0x8013A3E0, Size: 560 bytes (140 insns) */
#include "types.h"
#include "tokimeki/tt_box.h"

extern u8 tt_hit_box_tbl[];

extern u8 *tt_step_wk;
extern u8 *tt_player_wk;
extern u8 *tt_player_shot_buf;
extern u8 *tt_boss_wk;

extern void tt_cmd_once(s32 soundId);
extern s32 tt_box_overlap_check(u8 *a0, u8 *a1, s16 *a2, u8 *a3);
extern void tt_widget_pair_init(void);
extern void tt_shot_aimed_make(s16 x, s16 y, s32 w, s32 h);

void tt_cell_hit_scan(s16 *pos, u8 *tbl, s32 idx)
{
    u8 *cur;
    u8 *base;
    u8 *hit;
    u8 *obj;
    u8 *hp;
    s32 j;
    s32 d;
    s32 w;
    u32 cnt;
    u32 prev;

    cur = tt_player_wk;
    hit = tt_step_wk;
    base = tt_boss_wk;
    if (cur[82] == 0) {
        if (cur[0] != 0) {
            if (tt_box_overlap_check(cur + 20, tt_hit_box_tbl, pos, tbl) != 0) {
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
    }
    obj = tt_player_shot_buf;
    j = 0;
    do {
        if (obj[0] != 0 && *(u16 *)(obj + 4) == 64) {
            if (tt_box_overlap_check(obj + 20, ((u8 *)&((TT_BOX *)tt_hit_box_tbl)[1].x), pos, tbl) != 0) {
                cnt = hit[37] + 1;
                prev = hit[36];
                hit[37] = cnt;
                if (prev != 0 && (cnt & 3) == 0) {
                    tt_shot_aimed_make(*(s16 *)(obj + 20), *(s16 *)(obj + 22),
                                  32, 160);
                }
                tt_cmd_once(1288);
                *(s16 *)(obj + 4) = 128;
                *(s16 *)(obj + 10) = 0;
                *(s16 *)(obj + 8) = 0;
                *(s16 *)(obj + 6) = 0;
                obj[15] = 0;
                obj[14] = 0;
                obj[13] = 0;
                if (idx >= 0) {
                    d = idx * 2;
                    hp = base + d;
                    if (j >= 6) {
                        *(s16 *)(hp + 728) -= 1;
                    } else {
                        if (*(u16 *)(obj + 2) == 9) {
                            *(s16 *)(hp + 728) -= 4;
                        } else {
                            *(s16 *)(hp + 728) -= 2;
                        }
                    }
                    w = (idx >= 2) ? d + 2 : d;
                    *(base + (w * 20) + 66) = 1;
                    if (*(s16 *)(hp + 728) < 0) {
                        tt_cmd_once(1290);
                        *(base + idx + 835) = 1;
                    }
                }
            }
        }
        j += 1;
        obj += 84;
    } while (j != 12);
}
