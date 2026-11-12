/* tt_obj_init - TT overlay, Address: 0x80132FC8, Size: 144 bytes (36 insns) */
/* @PostProc: swap_instructions --offset1 0x10 --offset2 0x14 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:t6 0x14:rt:t6:t7 0x18:rt:t7:t8 0x1c:rd:t8:t9 0x20:rt:v1:t0 0x24:rt:t6:t7 0x30:rt:v0:t6 0x34:rt:t7:t8 0x38:rd:v1:t0 0x38:rs:v1:t0 0x38:rt:t8:t9 0x3c:rs:v1:t0 0x3c:rt:v1:t0 0x40:rt:t9:t1 0x48:rt:t9:t1 0x50:rs:v0:t6 0x54:rt:v1:t0 0x5c:rs:v0:t6 0x60:rt:t1:t3 0x68:rs:v0:t6 0x70:rs:v0:t6 0x74:rt:t2:t4 0x80:rt:t0:t2 0x88:rt:t0:t2 0x90:rt:t1:t3 0x94:rt:t2:t4 */
#include "types.h"

extern s32 tt_obj_anim_tbl[];

void tt_obj_init(u8 *a0, u32 a1, s32 a2)
{
    u16 sel;
    s32 tmp;

    sel = (u16)a1;
    a0[0] = 1;
    a0[81] = 0;
    a0[100] = 0;
    *(s16 *)(a0 + 2) = sel;
    *(s16 *)(a0 + 88) = 10;
    tmp = tt_obj_anim_tbl[a2];
    a0[64] = 128;
    *(s16 *)(a0 + 86) = 0;
    *(s32 *)(a0 + 56) = tmp;
    switch (sel) {
    case 96: *(s16 *)(a0 + 90) = 15559; break;
    case 97:
    case 98: *(s16 *)(a0 + 90) = 15560; break;
    case 99: *(s16 *)(a0 + 90) = 15561; break;
    }
}
