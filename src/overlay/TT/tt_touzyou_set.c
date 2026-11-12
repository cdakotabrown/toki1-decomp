/* tt_touzyou_set - TT overlay, Address: 0x80133058, Size: 128 bytes (32 insns) */
/* @PostProc: swap_instructions --offset1 0x28 --offset2 0x40 */
/* @PostProc: swap_instructions --offset1 0x34 --offset2 0x58 */
/* @PostProc: swap_instructions --offset1 0x70 --offset2 0x74 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:t6 0x18:rs:v0:t6 0x1c:rt:v0:t6 0x24:rs:v0:t6 0x30:rs:v0:t6 0x40:rt:t6:t8 0x44:rt:t8:t9 0x48:rt:t6:t8 0x54:rt:t8:t9 0x58:rt:t9:t1 0x5c:rt:t1:t2 0x60:rt:t9:t1 0x6c:rt:t1:t2 0x74:rt:t2:t4 0x78:rt:t4:t5 0x7c:rt:t2:t4 0x84:rt:t4:t5 */
#include "types.h"

void tt_touzyou_set(u8 *a0, u32 a1)
{
    u16 sel;

    sel = (u16)a1;
    *(s16 *)(a0 + 2) = sel;
    switch (sel) {
    case 110:
        a0[93] = 160;
        a0[92] = 240;
        *(s16 *)(a0 + 90) = 15492;
        break;
    case 111:
        a0[93] = 176;
        a0[92] = 240;
        *(s16 *)(a0 + 90) = 15492;
        break;
    case 109:
        a0[93] = 224;
        a0[92] = 240;
        *(s16 *)(a0 + 90) = 15501;
        break;
    }
}
