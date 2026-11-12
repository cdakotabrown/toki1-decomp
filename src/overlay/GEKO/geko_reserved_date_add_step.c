/* @PostProc: patch_insn_reg_field 0x3c:rt:a1:a0 0x48:rs:a1:a0 0x48:rt:a1:a0 0xac:rd:v1:t8 0xac:rs:a1:a0 0xb4:rs:v1:t8 0xc4:rt:t9:v0 0xc8:rd:t8:t7 0xcc:rd:t7:t6 0xcc:rt:t8:t7 0xd0:rd:t6:t9 0xd0:rt:t7:t6 0xd4:rt:t6:t9 0xd8:rs:t6:t9 0xe8:rt:t9:v0 0x1c0:rd:v0:v1 0x1c8:rt:a1:a0 0x1cc:rs:a0:v1 0x1d0:rt:v1:t5 0x1d4:rs:v1:t5 0x1d4:rt:v1:t5 0x1d8:rs:a1:a0 0x1d8:rt:a1:a0 0x1dc:rs:a1:a0 0x1dc:rt:v1:t5 0x1e0:rt:t5:t7 0x1e8:rs:t5:t7 0x1e8:rt:t7:t8 0x1ec:rt:t7:t8 0x1f0:rt:a1:a0 0x1f8:rs:a0:v1 0x1fc:rs:a1:a0 0x1fc:rt:a1:a0 0x200:rt:v1:t6 0x204:rs:v1:t6 0x204:rt:v1:t6 0x20c:rs:a1:a0 0x20c:rt:v1:t6 0x210:rt:t8:t9 0x218:rs:t8:t9 0x218:rt:t6:t2 0x21c:rt:t6:t2 0x224:rs:a0:v1 0x228:rt:v1:t0 0x22c:rs:v1:t0 0x22c:rt:v1:t0 0x234:rs:a1:a0 0x234:rt:v1:t0 0x238:rt:t9:t3 0x240:rs:t9:t3 0x240:rt:t2:t1 0x244:rt:t2:t1 0x1c4:rs:v0:v1 0x1c4:rt:a0:v1 */
/* Address: 0x80143E78 */
/* Size: 592 bytes (148 instructions) */
#include "types.h"

typedef struct {
    u8 b[21];
} GekoCmdBlk;

typedef struct {
    u32 cmd    : 5;
    u32 p0     : 7;
    u32 p1     : 4;
    u32 p2     : 5;
    u32 mode   : 3;
    u32 flagA  : 1;
    u32 girl   : 4;
    u32 flagB  : 1;
    u32 pad    : 2;
} GekoCmdWord;

extern s32 geko_week_end_get(s32 which);
extern void reserved_date_add(GekoCmdWord word);
extern void sub_sub_step_up(void);

extern u8 screen_w[];
extern volatile u8 now_girl;
extern s32 serifu_no;
extern s32 geko_ss_serifu3;
extern s32 geko_ss_serifu3_kind;
extern s32 geko_sss_sound3;
extern s16 geko_now_serifu3;
extern volatile u8 geko_pick_no;
extern GekoCmdBlk geko_reserved_date_add_step_tbl;
extern s32 geko_serifu3_tbl[];
extern s32 geko_serifu3_kind_tbl[];
extern s32 geko_sound3_tbl[];

void geko_reserved_date_add_step(void)
{
    u8 *rec;
    GekoCmdWord word;
    GekoCmdBlk blk;
    s16 lvl;
    u8 cmd;

    blk = geko_reserved_date_add_step_tbl;
    if (serifu_no != 0) {
        geko_ss_serifu3 = geko_serifu3_tbl[11];
        geko_ss_serifu3_kind = geko_serifu3_kind_tbl[11];
        geko_sss_sound3 = geko_sound3_tbl[11];
        rec = (u8 *)screen_w + now_girl * 56;
        lvl = (s16)(*(s16 *)(rec + 454) / 20);
        geko_now_serifu3 = lvl;
        if (lvl >= 5) {
            geko_now_serifu3 = 4;
        }
    } else {
        word.cmd = blk.b[geko_pick_no];
        word.p0 = geko_week_end_get(0);
        word.p1 = geko_week_end_get(1);
        word.p2 = geko_week_end_get(2);
        word.mode = 6;
        word.flagA = 1;
        word.girl = now_girl;
        word.flagB = 1;
        word.pad = 0;
        reserved_date_add(word);
        cmd = blk.b[geko_pick_no];
        if (cmd == 14) {
            screen_w[1868 + now_girl] |= 1;
        }
        if (cmd == 15) {
            screen_w[1868 + now_girl] |= 2;
        }
        if (cmd == 7) {
            screen_w[1868 + now_girl] |= 4;
        }
    }
    sub_sub_step_up();
}
