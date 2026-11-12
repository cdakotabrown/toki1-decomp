/* Address: 0x8015435C */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x24 */
/* @PostProc: swap_instructions --offset1 0x24 --offset2 0x2c */
/* @PostProc: swap_instructions --offset1 0x28 --offset2 0x3c */
/* @PostProc: patch_insn_reg_field 0x34:rd:s1:s5 0x40:rt:s3:s1 0x44:rt:s4:s2 0x48:rt:s5:s3 0x4c:rs:s1:s5 0x50:rt:s2:s6 0x58:rs:s5:s3 0x58:rt:s5:s3 0x5c:rs:s4:s2 0x5c:rt:s4:s2 0x60:rs:s3:s1 0x60:rt:s3:s1 0x64:rt:s6:s4 0x74:rs:s2:s6 0x80:rs:s3:s1 0x88:rs:s4:s2 0x90:rs:s5:s3 0xa4:rt:s6:s4 0xb4:rs:s1:s5 0xb4:rt:v0:t4 0xbc:rt:v0:t4 */
#include "types.h"

extern s32 *taco_node_prim_tbl[];

extern s32 taco_addr_check();

void taco_band_chan_set(a0, a1)
s32 a0;
u16 a1;
{
    s32 s1;
    s32 s2;
    s32 s3;
    s32 s4;
    s32 **s5;
    s32 *s0;
    s32 *end;
    s32 v0;
    s32 t1;
    s32 t2;
    s32 val;

    s1 = 0x24000507;
    s2 = 0x34000609;
    s3 = 0x24000014;
    s4 = 0x01E00000;
    s5 = &taco_node_prim_tbl[a0];
    s0 = s5[0];
    do {
        v0 = taco_addr_check(s0);
        if (v0 == 0) {
            break;
        }
        t1 = a1 & 0x1F;
        v0 = s0[0];
        t2 = t1 << 16;
        if (s1 == v0 || s2 == v0 || s3 == v0) {
            val = s0[2];
            s0 += 2;
            s0[0] = (val & 0xFFFF) + (val & s4) + t2;
        }
        end = s5[1];
        s0 += 1;
    } while ((u32)s0 < (u32)end);
}
