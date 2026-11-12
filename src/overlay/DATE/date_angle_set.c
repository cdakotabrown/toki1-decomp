/* Address: 0x8014AD0C, Size: 492 bytes (123 instructions) */
/* @PostProc: remap_stack_slots --map '40:44,44:48' */
/* @PostProc: patch_insn_reg_field 0x14:rt:t6:t8 0x20:rs:t6:t8 0x20:rt:t6:t8 0x24:rs:t6:t8 0x34:rs:t6:t8 0x34:rt:t9:t1 0x38:rd:t0:t6 0x3c:rt:t9:t1 0x40:rs:t6:t8 0x44:rd:t0:t6 0x44:rs:t0:t6 0x4c:rs:t6:t8 0x4c:rt:t9:t1 0x54:rt:t9:t1 0x58:rs:t6:t8 0x5c:rd:t1:t7 0x5c:rt:t0:t6 0x64:rs:t6:t8 0x64:rt:t9:t1 0x6c:rt:t9:t1 0x70:rs:t6:t8 0x74:rt:t1:t7 0x7c:rs:t6:t8 0x7c:rt:t9:t1 0x84:rt:t9:t1 0x8c:rt:t8:t5 0xa0:rt:t5:t6 0xa4:rt:t6:t9 0xa8:rt:t8:t6 0xb0:rt:t9:t0 0xb4:rt:t1:t3 0xb8:rd:t0:t8 0xb8:rs:t6:t0 0xbc:rt:t0:t8 0xc0:rt:t3:t1 0xc4:rt:t8:t6 0xc8:rs:t8:t6 0xc8:rt:t8:t6 0xd8:rt:t8:t6 0xdc:rt:t6:t7 0xe0:rt:t7:t0 0xe8:rt:t6:t0 0xf8:rd:t0:t8 0xfc:rs:t0:t8 0x11c:rt:t4:t3 0x120:rt:t3:t4 0x15c:rt:t6:t7 0x160:rt:t7:t0 0x168:rt:t6:t0 0x178:rd:t0:t8 0x17c:rs:t0:t8 */
#include "types.h"

typedef struct {
    s32 w[8];
} DateBlock;

typedef struct {
    s32 unk00;
    DateBlock block;
    s32 unk24[11];
} DatePayload;

typedef struct {
    s16 angle0;
    s16 angle1;
    s16 angle2;
    s16 unk06;
} DateAngles;

extern DateBlock GsIDMATRIX;
extern DatePayload coord_buf[];
extern DateAngles date_angle_tbl[];

extern void RotMatrixX(s32 arg0, DateBlock *arg1);
extern void RotMatrixZ(s32 arg0, DateBlock *arg1);
extern void RotMatrixY(s32 arg0, DateBlock *arg1);

void date_angle_set(s32 idx, s16 *src, s32 row)
{
    DateBlock blk;
    DatePayload *pay;
    DateAngles *ang;

    blk = GsIDMATRIX;
    src += row * 6;

    pay = &coord_buf[idx];
    blk.w[5] = src[0] + pay->block.w[5];
    blk.w[6] = src[1] + pay->block.w[6];
    blk.w[7] = src[2] + pay->block.w[7];

    ang = &date_angle_tbl[idx];
    ang->angle0 = ang->angle0 + src[3];
    src += 4;
    RotMatrixX(((s32)ang->angle0 << 12) / 360, &blk);

    ang->angle2 = ang->angle2 + src[0];
    src += 1;
    RotMatrixZ(((s32)ang->angle2 << 12) / 360, &blk);

    ang->angle1 = ang->angle1 + src[0];
    RotMatrixY(((s32)ang->angle1 << 12) / 360, &blk);

    pay->block = blk;
    pay->unk00 = 0;
}
