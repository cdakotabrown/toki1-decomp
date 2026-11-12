/* @SymbolSize: 172 */
/* @PostProc: patch_insn_reg_field 0x10:rt:t7:t8 0x14:rs:t7:t8 0x14:rt:t7:t8 0x18:rd:v1:t6 0x1c:rt:v1:t6 0x24:rd:t8:t9 0x24:rs:t7:t8 0x28:rt:t8:t9 0x2c:rt:t9:t0 0x30:rs:t9:t0 0x30:rt:t9:t0 0x38:rd:t0:t1 0x38:rs:t9:t0 0x3c:rt:t0:t1 0x40:rt:t1:t2 0x44:rs:t1:t2 0x44:rt:t1:t2 0x4c:rd:t2:t3 0x4c:rs:t1:t2 0x50:rt:t2:t3 0x54:rt:t3:t4 0x58:rs:t3:t4 0x58:rt:t3:t4 0x60:rd:t4:t5 0x60:rs:t3:t4 0x64:rt:t4:t5 0x68:rt:t5:t6 0x6c:rs:t5:t6 0x6c:rt:t5:t6 0x74:rd:t6:t7 0x74:rs:t5:t6 0x78:rt:t6:t7 0x7c:rt:t7:t8 0x80:rs:t7:t8 0x80:rt:t7:t8 0x88:rd:t8:t9 0x88:rs:t7:t8 0x8c:rt:t8:t9 0x90:rt:t9:t0 0x94:rs:t9:t0 0x94:rt:t9:t0 0x9c:rd:t0:t1 0x9c:rs:t9:t0 0xa0:rt:t0:t1 0xa4:rt:t1:t2 0xa8:rs:t1:t2 0xa8:rt:t1:t2 0xb0:rd:t2:t3 0xb0:rs:t1:t2 0xb8:rt:t2:t3 */
/* Function: date_box_move */
/* Overlay: DATE */
/* Address: 0x80148894 */
/* Size: 172 bytes (43 instructions) */
#include "types.h"

extern volatile s16 sprite4_y;
extern volatile u16 sprite4_h;
extern volatile s16 sprite5_y;
extern volatile u16 sprite5_h;
extern volatile u16 sprite6_h;
extern volatile u8  sprite6_v;
extern volatile u16 sprite7_h;
extern volatile u8  sprite7_v;

s32 date_box_move(s32 arg)
{
    s32 delta;

    delta = (s16)arg;
    sprite4_y = sprite4_y + delta;
    sprite5_y = sprite5_y + delta;
    sprite4_h = sprite4_h - delta;
    sprite5_h = sprite5_h - delta;
    sprite6_h = sprite6_h + delta;
    sprite7_h = sprite7_h + delta;
    sprite6_v = sprite6_v - delta;
    sprite7_v = sprite7_v - delta;

    return delta;
}
