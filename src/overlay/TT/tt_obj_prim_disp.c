/* @SymbolStart: 0x8014CFDC */
/* @SymbolSize: 380 */
/* @PostProc: swap_instructions --offset1 0x6c --offset2 0x70 */
/* @PostProc: patch_insn_reg_field 0x78:rt:v0:t5 0x80:rs:v0:t5 0x80:rt:t5:t6 0x84:rs:t5:t6 0x88:rt:v0:t7 0x8c:rs:v0:t7 0x8c:rt:v0:t7 0x94:rs:v0:t7 0x94:rt:t6:t8 0x9c:rt:t6:t8 0xa0:rs:v0:t7 0xa0:rt:t7:t9 0xa8:rt:t7:t9 0xac:rs:v0:t7 0xac:rt:t8:t0 0xb8:rt:t8:t0 0xbc:rt:t9:t1 0xc4:rt:t9:t1 0xc8:rt:t0:t2 0xd0:rt:t0:t2 0xd4:rt:t1:t3 0xdc:rt:t1:t3 0xe0:rt:t2:t4 0xe8:rt:t2:t4 0xec:rt:t3:t5 0xf4:rt:t3:t5 0xf8:rt:t4:t6 0x100:rt:t4:t6 0x104:rt:t5:t7 0x10c:rt:t5:t7 0x110:rt:t6:t8 0x118:rt:t6:t8 0x11c:rt:t7:t9 0x124:rt:t7:t9 0x128:rt:t8:t0 0x130:rt:t8:t0 0x134:rt:t9:t1 0x13c:rt:t9:t1 0x140:rt:t0:t2 0x148:rt:t0:t2 0x14c:rt:t1:t3 0x154:rt:t1:t3 0x158:rt:t2:t4 0x160:rt:t2:t4 0x164:rt:t3:t5 0x16c:rd:t4:t6 0x16c:rt:t3:t5 0x170:rs:t4:v1 0x170:rt:v1:t6 */
/* Address: 0x8014CFDC (cross-prologue head absorbed), Size: 380 bytes (95 insns) */
#include "types.h"

extern void tt_sprite_ot_add_var(u8 *ot, u8 *prim, s32 arg);

extern u8 *volatile tt_step_wk;
extern u8 *tt_disp_wk;
extern u8 *tt_db_buf;

void tt_obj_prim_disp(u8 *obj)
{
    u8 *hdr;
    u8 *base;
    u8 *p;
    u8 *q;
    u8 *o;
    u32 flags;

    hdr = tt_disp_wk;
    base = tt_db_buf + *(s32 *)(hdr + 8) * 32952;
    p = *(u8 **)(base + 4);
    *(u8 **)(base + 4) = p + 48;

    *(s16 *)(p + 8) = *(s16 *)(obj + 20);
    *(s16 *)(p + 10) = *(s16 *)(obj + 22);
    p[12] = 56;
    p[13] = 184;

    flags = *(u16 *)(obj + 68);
    o = obj;
    if (flags & 8) {
        q = tt_step_wk;
        p[4] = q[29];
        p[5] = q[30];
        p[6] = q[31];
        p[42] = 0;
    } else {
        p[4] = o[56];
        p[5] = o[57];
        p[6] = o[58];
        p[42] = o[78];
    }

    p[41] = o[77];
    p[43] = o[79];
    p[44] = o[80];
    *(s16 *)(p + 16) = *(s16 *)(o + 60);
    *(s16 *)(p + 18) = *(s16 *)(o + 62);
    p[12] = o[70];
    p[13] = o[71];
    *(s16 *)(p + 22) = *(u16 *)(o + 64);
    *(s16 *)(p + 14) = *(u16 *)(o + 66);
    p[40] = o[76];

    tt_sprite_ot_add_var(base + (*(u16 *)(o + 16) << 2) + 0x8008, p,
                  *(s32 *)(o + 72));
}
