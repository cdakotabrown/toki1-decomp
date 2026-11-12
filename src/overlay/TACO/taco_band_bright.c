/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x24 */
/* @PostProc: swap_instructions --offset1 0x1c --offset2 0x28 */
/* @PostProc: swap_instructions --offset1 0x20 --offset2 0x2c */
/* @PostProc: swap_instructions --offset1 0x30 --offset2 0x54 */
/* @PostProc: swap_instructions --offset1 0x30 --offset2 0x50 */
/* @PostProc: swap_instructions --offset1 0x30 --offset2 0x4c */
/* @PostProc: swap_instructions --offset1 0x30 --offset2 0x48 */
/* @PostProc: swap_instructions --offset1 0x30 --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x30 --offset2 0x40 */
/* @PostProc: swap_instructions --offset1 0x30 --offset2 0x34 */
/* @PostProc: swap_instructions --offset1 0x30 --offset2 0x38 */
/* @PostProc: swap_instructions --offset1 0x30 --offset2 0x3c */
/* @PostProc: swap_instructions --offset1 0xac --offset2 0xb4 */
/* @PostProc: swap_instructions --offset1 0xb0 --offset2 0xc0 */
/* @PostProc: swap_instructions --offset1 0xb0 --offset2 0xbc */
/* @PostProc: swap_instructions --offset1 0xb8 --offset2 0xc8 */
/* @PostProc: swap_instructions --offset1 0xb8 --offset2 0xd0 */
/* @PostProc: swap_instructions --offset1 0xb8 --offset2 0xd8 */
/* @PostProc: swap_instructions --offset1 0xb8 --offset2 0xdc */
/* @PostProc: swap_instructions --offset1 0xb8 --offset2 0xe0 */
/* @PostProc: swap_instructions --offset1 0xb8 --offset2 0xd4 */
/* @PostProc: swap_instructions --offset1 0xb8 --offset2 0xc4 */
/* @PostProc: swap_instructions --offset1 0xb8 --offset2 0xcc */
/* @PostProc: swap_instructions --offset1 0xe4 --offset2 0xe8 */
/* @PostProc: patch_insn_reg_field 0x78:rt:v1:v0 0x80:rs:v1:s3 0x80:rt:s3:v0 0x88:rs:v1:s4 0x88:rt:s4:v0 0x90:rs:v1:s5 0x90:rt:s5:v0 0x98:rt:v1:v0 0xa0:rd:t8:v1 0xa0:rt:v1:v0 0xa4:rs:t8:v1 0xa4:rt:t9:t8 0xa8:rd:a2:v1 0xa8:rs:t9:t8 0xac:rd:t0:a0 0xac:rt:v1:v0 0xb0:rs:t0:a0 0xb0:rt:t1:t9 0xb4:rs:a2:v1 0xb8:rs:v1:v0 0xb8:rt:t2:a1 0xbc:rs:t1:t9 0xc4:rs:t2:a1 0xc8:rt:a2:v1 0xd4:rd:t3:t0 0xd4:rs:v1:v0 0xe4:rs:a2:v1 0xe4:rt:t4:t1 0xec:rd:t5:t2 0xec:rt:t4:t1 0xf0:rt:t7:t4 0xf4:rd:t8:t5 0xf4:rt:t7:t4 0xf8:rd:t6:t3 0xf8:rs:t3:t0 0xf8:rt:t5:t2 0xfc:rd:t9:t6 0xfc:rs:t6:t3 0xfc:rt:t8:t5 0x100:rt:t0:t7 0x104:rd:t1:t8 0x104:rs:t9:t6 0x104:rt:t0:t7 0x108:rt:t1:t8 0x10c:rt:t2:t9 0x114:rt:t2:t9 */
/* Address: 0x80154820   BYTE-PERFECT (80/80 words, len t=c=320, 0 unresolved) */
#include "types.h"

extern s32 taco_addr_check();
extern u32 *taco_node_prim_tbl[];

void
taco_band_bright(s32 idx, s32 ofs)
{
    u32 **base;
    u32 *p;
    u32 c;
    s32 r;
    s32 g;
    s32 b;

    base = &taco_node_prim_tbl[idx];
    p = base[0];
    do {
        if (taco_addr_check(p) == 0) {
            return;
        }
        c = *p;
        if (c == 0x21010001 || c == 0x21010002 || c == 0x21010304) {
            c = p[1];
            p++;
            r = ((c >> 16) & 0xFF) + ofs;
            if (r >= 256) {
                r = 255;
            }
            g = ((c >> 8) & 0xFF) + ofs;
            if (g >= 256) {
                g = 255;
            }
            b = (c & 0xFF) + ofs;
            if (b >= 256) {
                b = 255;
            }
            *p = (c & 0xFF000000) + ((r & 0xFF) << 16) +
                 ((g & 0xFF) << 8) + (b & 0xFF);
        }
        p++;
    } while (p < base[1]);
}
