/* @SymbolSize: 592 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: patch_stack_complete --old 64 --new 72 */
/* @PostProc: remap_stack_slots --from 56 --to 60 */
/* @PostProc: swap_instructions --offset1 0x194 --offset2 0x198 */
/* @PostProc: patch_insn_reg_field 0x28:rt:v0:t7 0x2c:rt:t8:t9 0x30:rd:t7:t8 0x30:rt:v0:t7 0x34:rd:t7:t8 0x34:rs:t7:t8 0x34:rt:v0:t7 0x38:rd:t7:t8 0x38:rt:t7:t8 0x3c:rd:t7:t8 0x3c:rs:t7:t8 0x3c:rt:v0:t7 0x40:rd:t7:t8 0x40:rt:t7:t8 0x44:rs:t8:t9 0x44:rt:t8:t9 0x48:rt:t9:t2 0x4c:rd:t7:t8 0x4c:rs:t7:t8 0x4c:rt:v0:t7 0x50:rd:t7:t8 0x50:rt:t7:t8 0x5c:rs:t9:t2 0x60:rd:t0:t1 0x60:rs:t7:t8 0x60:rt:t8:t9 0x64:rt:t1:t3 0x6c:rs:t1:t3 0x74:rt:t2:t4 0x78:rt:t3:t5 0x7c:rs:t2:t4 0x80:rt:t4:t6 0x84:rt:t5:t7 0x88:rt:t3:t5 0x8c:rs:t5:t7 0x90:rt:t4:t6 0x94:rs:t4:t6 0x94:rt:t7:t9 0x9c:rt:t7:t9 0xa0:rt:t8:t2 0xa8:rs:t8:t2 0xa8:rt:t9:t3 0xac:rt:t9:t3 0xb0:rt:t1:t4 0xb8:rs:t1:t4 0xc0:rt:t2:t5 0xc8:rs:t2:t5 0xc8:rt:t3:t6 0xd0:rt:t3:t6 0xd4:rt:t4:t7 0xdc:rs:t4:t7 0xdc:rt:t5:t8 0xe0:rt:t5:t8 0xe4:rt:t6:t9 0xec:rd:t7:t2 0xec:rs:t6:t9 0xf0:rd:t8:t3 0xf0:rs:t7:t2 0xf4:rd:t9:t4 0xf4:rt:t8:t3 0xf8:rd:t1:t5 0xf8:rt:t9:t4 0xfc:rd:t2:t6 0xfc:rs:t1:t5 0x100:rt:t2:t6 0x108:rd:t3:t7 0x10c:rd:t4:t8 0x10c:rs:t3:t7 0x110:rd:t5:t9 0x110:rt:t4:t8 0x114:rd:t6:t2 0x114:rt:t5:t9 0x118:rd:t7:t3 0x118:rs:t6:t2 0x11c:rt:t7:t3 0x120:rs:t0:t1 0x128:rt:t8:t4 0x12c:rs:t0:t1 0x12c:rt:t8:t4 0x148:rt:t9:t5 0x158:rd:t1:t6 0x158:rs:t9:t5 0x15c:rd:t2:t7 0x15c:rs:t1:t6 0x160:rd:t3:t8 0x160:rt:t2:t7 0x164:rd:t4:t9 0x164:rt:t3:t8 0x168:rd:t5:t2 0x168:rs:t4:t9 0x16c:rt:t5:t2 0x174:rd:t6:t3 0x178:rd:t7:t4 0x178:rs:t6:t3 0x17c:rd:t8:t5 0x17c:rt:t7:t4 0x180:rd:t9:t6 0x180:rt:t8:t5 0x184:rd:t1:t7 0x184:rs:t9:t6 0x18c:rt:t1:t7 0x190:rt:t4:t2 0x194:rt:t3:t8 0x198:rt:t2:t9 0x19c:rt:t2:t9 0x1a0:rs:t4:t2 0x1a4:rt:t3:t8 0x1a8:rs:t2:t9 0x1a8:rt:t6:t4 0x1b0:rt:t6:t4 0x1b4:rt:t7:t5 0x1bc:rs:t7:t5 0x1bc:rt:t8:t6 0x1c0:rt:t8:t6 0x1c4:rt:t9:t7 0x1cc:rs:t9:t7 0x1d4:rt:t1:t8 0x1dc:rs:t1:t8 0x1dc:rt:t2:t9 0x1e4:rt:t2:t9 0x1e8:rt:t3:t2 0x1f0:rs:t3:t2 0x1f0:rt:t4:t3 0x1f4:rt:t4:t3 0x208:rt:t5:t4 0x218:rd:t6:t5 0x218:rs:t5:t4 0x21c:rd:t7:t6 0x21c:rs:t6:t5 0x220:rd:t8:t7 0x220:rt:t7:t6 0x224:rd:t9:t8 0x224:rt:t8:t7 0x228:rd:t1:t9 0x228:rs:t9:t8 0x22c:rt:t1:t9 */
/* Address: 0x80141A0C, Size: 592 bytes (148 instructions) */
#include "types.h"

typedef struct {
    u32 addr : 24;
    u32 len  : 8;
} PTag;

typedef struct {
    PTag t;
    u8   r0;
    u8   g0;
    u8   b0;
    u8   code;
    u8   pad08[8];
    s16  unk10;
    s16  unk12;
    u8   pad14[2];
    u16  unk16;
    u8   pad18[16];
    u8   unk28;
    u8   unk29;
    u8   unk2A;
    u8   unk2B;
    u8   unk2C;
} Obj;

extern u8 *tt_disp_wk;
extern u8 *tt_db_buf;

extern void SetDrawMode(PTag *, s32, s32, s32, s32);
extern void tt_sprite_quad_set(Obj *, s32);

void tt_sprite_ot_add_8x8(PTag *ot, Obj *p, s32 arg3)
{
    u8 *heap;
    PTag *blk;
    s32 idx;

    idx = *(s32 *)(tt_disp_wk + 8);
    heap = tt_db_buf + idx * 32952;

    if (p->unk2C == 0 && p->unk2B == 0 && p->unk28 == 0) {
        p->t.len = 3;
        p->code = 116;
        if (p->unk29 != 0) {
            p->code |= 2;
        } else {
            p->code &= ~2;
        }
        if (p->unk2A != 0) {
            p->code |= 1;
        } else {
            p->code &= ~1;
        }
        p->t.addr = ot->addr;
        ot->addr = (u32)p;

        blk = *(PTag **)(heap + 4);
        *(PTag **)(heap + 4) = (PTag *)((u8 *)blk + 12);
        SetDrawMode(blk, 0, 0, p->unk16, 0);
        blk->addr = ot->addr;
        ot->addr = (u32)blk;
    } else {
        p->code = 44;
        p->t.len = 9;
        if (p->unk29 != 0) {
            p->code |= 2;
        } else {
            p->code &= ~2;
        }
        if (p->unk2A != 0) {
            p->code |= 1;
        } else {
            p->code &= ~1;
        }
        p->unk10 = 8;
        p->unk12 = 8;
        tt_sprite_quad_set(p, arg3);
        p->t.addr = ot->addr;
        ot->addr = (u32)p;
    }
}
