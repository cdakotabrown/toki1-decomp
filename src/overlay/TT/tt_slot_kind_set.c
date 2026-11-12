/* @RodataBase: 0x8014D810 */
/* @PostProc: swap_instructions --offset1 0x10 --offset2 0x14 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:t6 0x14:rt:t6:t7 0x18:rt:t7:t8 0x1c:rd:t8:t9 0x20:rt:v1:t0 0x24:rt:t6:t7 0x2c:rt:v0:t6 0x30:rt:t7:t8 0x34:rd:v1:t0 0x34:rs:v1:t0 0x34:rt:t8:t9 0x38:rs:v1:t0 0x38:rt:v1:t0 0x3c:rs:v0:t6 0x3c:rt:t0:t2 0x40:rt:t9:t1 0x44:rs:t0:t2 0x48:rt:t9:t1 0x50:rt:v1:t0 0x54:rd:t0:t2 0x54:rt:t0:t2 0x5c:rt:t0:t2 0x60:rt:t0:t2 0x68:rs:t0:t2 0x70:rt:t1:t3 0x78:rt:t1:t3 0x7c:rt:t2:t4 0x84:rt:t2:t4 0x88:rt:t3:t5 0x90:rt:t3:t5 0x94:rt:t4:t6 0x9c:rt:t4:t6 0xa0:rt:t5:t7 0xa8:rt:t5:t7 0xac:rt:t6:t8 0xb4:rt:t6:t8 0xb8:rt:t7:t9 0xc0:rt:t7:t9 0xc4:rt:t8:t0 0xcc:rt:t8:t0 0xd0:rt:t9:t1 0xd4:rt:t9:t1 */
/* tt_slot_kind_set - TT overlay, Address: 0x8014AFA8, Size: 208 bytes (52 insns) */
#include "types.h"

extern s32 tt_obj_anim_tbl[];

void tt_slot_kind_set(u8 *a0, u32 a1, s32 a2)
{
    u16 sel;
    s32 tmp;

    sel = (u16)a1;
    a0[0] = 1;
    a0[81] = 0;
    *(s16 *)(a0 + 2) = sel;
    *(s16 *)(a0 + 88) = 10;
    tmp = tt_obj_anim_tbl[a2];
    a0[64] = 128;
    *(s32 *)(a0 + 56) = tmp;
    switch ((u32)sel) {
    case 64: *(s16 *)(a0 + 90) = 15552; break;
    case 65: *(s16 *)(a0 + 90) = 15553; break;
    case 66: *(s16 *)(a0 + 90) = 15553; break;
    case 67: *(s16 *)(a0 + 90) = 15554; break;
    case 68: *(s16 *)(a0 + 90) = 15555; break;
    case 69: *(s16 *)(a0 + 90) = 15557; break;
    case 70: *(s16 *)(a0 + 90) = 15561; break;
    case 71: *(s16 *)(a0 + 90) = 15558; break;
    case 72: *(s16 *)(a0 + 90) = 15557; break;
    }
}
