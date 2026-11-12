/* @CompileFlags: -Wc,-pic0 */
/* @PostProc: swap_instructions --offset1 0xd4 --offset2 0xec */
/* @PostProc: swap_instructions --offset1 0xe0 --offset2 0xec */
/* @PostProc: swap_instructions --offset1 0xe8 --offset2 0xec */
/* @PostProc: patch_insn_reg_field 0xa8:rt:t3:t4 0xac:rs:t3:t4 0xac:rt:t3:t4 0xb0:rt:t7:a1 0xb4:rd:t4:t5 0xb4:rt:t3:t4 0xb8:rd:t5:t8 0xb8:rt:t4:t5 0xbc:rs:t5:t8 0xbc:rt:t8:t9 0xc0:rt:t9:t6 0xc4:rd:t6:t7 0xc4:rs:t9:t6 0xc4:rt:t8:t9 0xc8:rs:t6:t7 0xc8:rt:t7:a1 0xd4:rt:t1:a2 0xf0:rt:t5:t3 0x104:rt:t1:a2 0x108:rd:t2:t1 0x10c:rd:t3:t2 0x10c:rt:t2:t1 0x110:rs:t3:t2 0x114:rt:t3:t2 0x118:rs:t3:t2 0x120:rd:t9:t5 0x120:rs:t5:t3 0x128:rt:t9:t5 0x134:rt:t1:t0 0x13c:rd:t7:t9 0x140:rs:t7:t9 0x140:rt:t0:t7 0x148:rs:t1:t0 0x148:rt:t0:t7 0x14c:rd:t2:t1 0x150:rt:t2:t1 0x164:rt:t0:t7 0x170:rt:t7:t9 0x178:rd:t1:t0 0x178:rt:t7:t9 0x17c:rs:t1:t0 0x17c:rt:t0:t7 0x180:rt:t3:t2 0x184:rt:t5:t3 0x188:rs:t5:t3 0x188:rt:t5:t3 0x18c:rt:t9:t5 0x190:rt:t5:t3 0x194:rt:t5:t3 0x19c:rs:t9:t5 0x19c:rt:t9:t5 0x1a0:rt:t9:t5 0x1a4:rd:t7:t9 0x1a8:rd:t1:t0 0x1a8:rt:t7:t9 0x1ac:rd:t2:t1 0x1b0:rs:t1:t0 0x1b0:rt:t0:t7 0x1b8:rs:t2:t1 0x1b8:rt:t3:t2 0x1bc:rt:t0:t7 0x1c0:rd:t2:t1 0x1c4:rt:t2:t1 0x1d0:rd:t5:t3 0x1d4:rt:t5:t3 0x1dc:rd:t9:t5 0x1e4:rd:t9:t5 0x1e8:rt:t9:t5 0x1f4:rt:t9:t5 0x1f8:rs:t9:t5 0x1f8:rt:t9:t5 0x1fc:rt:t3:t2 0x200:rt:t9:t5 0x204:rd:t7:t9 0x208:rs:t7:t9 0x208:rt:t1:t0 0x20c:rt:t5:t3 0x210:rd:t0:t7 0x210:rt:t1:t0 0x214:rd:t2:t1 0x214:rt:t0:t7 0x218:rs:t2:t1 0x218:rt:t2:t1 0x21c:rs:t5:t3 0x21c:rt:t5:t3 0x220:rs:t2:t1 0x220:rt:t3:t2 0x224:rt:t5:t3 0x22c:rt:t5:t3 0x240:rt:t7:t9 0x244:rd:t5:t3 0x250:rs:t5:t3 0x258:rd:t9:t5 0x25c:rs:t9:t5 0x260:rt:t9:t5 0x264:rs:t9:t5 0x26c:rd:t1:t0 0x26c:rs:t7:t9 0x274:rt:t1:t0 */
/* Function: date_score_apply */
/* Overlay: DATE */
/* Address: 0x8013C12C */
/* Size: 640 bytes (160 instructions) */
#include "types.h"

extern s32 bustup_face_change();
extern void date_rank_grade_get(s32 a0, s32 *rank, s32 *grade);
extern s32 printf();
extern s32 param_clamp();
extern s32 sub_sub_step_up();

typedef struct {
    s32 v[5];
} DateWeights;

typedef struct {
    DateWeights w;
    s32 tail[2];
} DateWeightSlot;

extern DateWeights date_score_apply_tbl;
extern u8 date_zatu_basho_fmt[];
extern s32 serifu_no;
extern u8 kao[];
extern volatile s16 imp[];
extern u8 now_girl;
extern u8 screen_w[];

void date_score_apply(void)
{
    s32 slot0;
    s32 rank;
    s32 grade;
    DateWeightSlot tbl;
    s32 *rec;
    s32 id;
    s32 slot1;
    s32 slot2;

    tbl.w = date_score_apply_tbl;
    bustup_face_change(kao[serifu_no * 2]);

    date_rank_grade_get((s16)(3 - imp[serifu_no]), &rank, &grade);
    id = now_girl;
    rec = (s32 *)(screen_w + id * 56);
    rec[111] += (((3 - imp[serifu_no]) * rank * grade) << 16) / 16;
    printf(date_zatu_basho_fmt);

    date_rank_grade_get((s16)(2 - imp[serifu_no]), &rank, &grade);
    id = now_girl;
    rec = (s32 *)(screen_w + id * 56);
    rec[112] += (((2 - imp[serifu_no]) * rank * grade) << 16) / 16;

    date_rank_grade_get((s16)(2 - *(volatile s16 *)((u8 *)imp
                                              + (serifu_no << 1))),
                  &rank, &grade);
    id = now_girl;
    rec = (s32 *)(screen_w + id * 56);
    rec[113] += (((tbl.w.v[imp[serifu_no]] * rank * grade) << 16) / 16);

    param_clamp();
    sub_sub_step_up();
}
