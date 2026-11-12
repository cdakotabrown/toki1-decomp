/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: patch_stack_complete --old 104 --new 96 */
/* @PostProc: swap_branch_operands --offsets 0x70,0xb4 */
/* @PostProc: patch_insn_reg_field 0x60:rt:v1:t6 0x68:rs:v1:t6 0x78:rt:t6:t7 0x80:rs:t6:t7 0xa0:rt:v1:t8 0xa8:rs:v1:t8 0xa8:rt:a0:t9 0xb0:rs:a0:t9 0xb0:rt:a1:v1 0xb4:rs:a1:v1 0xb8:rt:a0:t9 0xbc:rs:a1:v1 0xc4:rs:a1:v1 0xcc:rs:a1:v1 0xd8:rt:v1:t0 0xf8:rt:v1:t0 0x100:rs:v1:t0 0x100:rt:t7:t1 0x108:rd:t8:t2 0x108:rt:t7:t1 0x10c:rt:t8:t2 0x110:rt:v1:t3 0x114:rt:t2:t9 0x118:rs:v1:t3 0x118:rt:v0:t4 0x120:rd:t9:t5 0x120:rt:v0:t4 0x124:rt:t9:t5 0x128:rt:t0:t6 0x130:rd:t1:t8 0x130:rs:t0:t6 0x130:rt:t9:t5 0x134:rt:t1:t8 0x138:rt:t3:t0 0x148:rt:t2:t9 0x150:rt:t3:t0 0x154:rt:t4:t1 0x15c:rs:t4:t1 0x15c:rt:t5:t2 0x160:rt:t5:t2 0x17c:rt:t6:t3 0x184:rs:t6:t3 0x184:rt:t7:t4 0x18c:rt:t7:t4 */
/* tt_obj_spawn - TT overlay, Address: 0x801330D8, Size: 432 bytes (108 insns) */
#include "types.h"

extern u8 *tt_stage_wk;
extern u8 *tt_obj_pool;

extern u8 *get_tt_free_rec(u8 *a0, u8 *a1);
extern void tt_obj_init(u8 *a0, u16 a1, s32 a2);

void tt_obj_spawn(void)
{
    u8 *p;
    u8 *obj;
    u8 *e;
    u8 *base;
    s32 kind;
    s32 tmp;
    s16 v;
    s32 w;

    p = tt_stage_wk;
    for (;;) {
        e = *(u8 **)(p + 8);
        v = *(s16 *)(e + 2);
        if (v == -1) {
            return;
        }
        if ((u32)*(u16 *)(p + 2) < (u32)v) {
            return;
        }
        base = tt_obj_pool;
        obj = get_tt_free_rec(base, base + 3840);
        if (obj == 0) {
            return;
        }
        e = *(u8 **)(p + 8);
        w = *(u16 *)(e + 4);
        *(s16 *)(obj + 2) = w;
        switch ((u16)w) {
        case 96:
            kind = 14;
            break;
        case 97:
            kind = 15;
            break;
        case 98:
            kind = 16;
            break;
        case 99:
            kind = 17;
            break;
        }
        e = *(u8 **)(p + 8);
        *(s32 *)(obj + 32) = *(s16 *)(e + 0) << 16;
        e = *(u8 **)(p + 8);
        tmp = *(s16 *)(e + 2) << 16;
        *(s32 *)(obj + 28) = tmp;
        *(s32 *)(obj + 36) = *(s32 *)(p + 0) - tmp;
        *(s32 *)(obj + 40) = 0;
        *(s32 *)(obj + 44) = 0;
        *(s16 *)(obj + 94) = 0;
        obj[96] = 0;
        *(s16 *)(obj + 20) = *(s16 *)(obj + 34);
        *(s16 *)(obj + 22) = *(s16 *)(obj + 38);
        tt_obj_init(obj, *(u16 *)(obj + 2), kind);
        obj[64] |= 0x40;
        *(s16 *)(obj + 4) = 64;
        *(s16 *)(obj + 10) = 0;
        *(s16 *)(obj + 8) = 0;
        *(s16 *)(obj + 6) = 0;
        obj[15] = 0;
        obj[14] = 0;
        obj[13] = 0;
        *(s32 *)(p + 8) = *(s32 *)(p + 8) + 6;
    }
}
