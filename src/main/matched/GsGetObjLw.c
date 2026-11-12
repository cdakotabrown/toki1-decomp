/* Address: 0x8009EF9C */
/* Size: 632 bytes (158 instructions) */
/* Region: Library (0x80086000-0x800BFFFF) */
/* @Compiler: gcc272 */
/* @CompileFlags: --fill-epilogue --load-dest-temp --jal-load-nop */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_insn_reg_field --patch 0xa4:rd:s6:s3 --patch 0xa8:rt:v0:a0 --patch 0xac:rt:v0:a0 --patch 0xb0:rd:s3:s6 --patch 0xd4:rs:s6:s3 --patch 0xd8:rs:s6:s3 --patch 0xdc:rs:s6:s3 --patch 0xe0:rs:s6:s3 --patch 0xf4:rs:s6:s3 --patch 0xf8:rs:s6:s3 --patch 0xfc:rs:s6:s3 --patch 0x100:rs:s6:s3 --patch 0x13c:rs:s6:s3 --patch 0x13c:rt:s6:s3 --patch 0x140:rs:s3:s6 --patch 0x140:rt:s3:s6 --patch 0x144:rs:s3:s6 */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0x10 */
/* @PostProc: swap_instructions --offset1 0xc --offset2 0x1c */
/* @PostProc: swap_instructions --offset1 0x10 --offset2 0x20 */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x24 */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x28 */
/* @PostProc: swap_instructions --offset1 0x1c --offset2 0x20 */
/* @PostProc: swap_instructions --offset1 0x20 --offset2 0x2c */
/* @PostProc: swap_instructions --offset1 0x24 --offset2 0x30 */
/* @PostProc: swap_instructions --offset1 0x28 --offset2 0x2c */
/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x44 --offset2 0x48 */
/* @PostProc: swap_instructions --offset1 0x64 --offset2 0x6c */
/* @PostProc: swap_instructions --offset1 0x70 --offset2 0x78 */
/* @PostProc: swap_instructions --offset1 0x7c --offset2 0x80 */
/* @PostProc: swap_instructions --offset1 0x84 --offset2 0x88 */
/* @PostProc: swap_instructions --offset1 0xa4 --offset2 0xa8 */
/* @PostProc: swap_instructions --offset1 0xa8 --offset2 0xac */
/* @PostProc: swap_instructions --offset1 0xac --offset2 0xb0 */
/* @PostProc: swap_instructions --offset1 0xa8 --offset2 0xac */
/* @PostProc: swap_instructions --offset1 0xa4 --offset2 0xa8 */
/* @PostProc: swap_instructions --offset1 0xa0 --offset2 0xa4 */
/* @PostProc: swap_instructions --offset1 0x9c --offset2 0xa0 */
/* @PostProc: swap_instructions --offset1 0x98 --offset2 0x9c */
/* @PostProc: swap_instructions --offset1 0x94 --offset2 0x98 */
/* @PostProc: swap_instructions --offset1 0x90 --offset2 0x94 */
/* @PostProc: swap_instructions --offset1 0xb4 --offset2 0xb8 */
/* @PostProc: swap_instructions --offset1 0xf4 --offset2 0x100 */
/* @PostProc: swap_instructions --offset1 0xf8 --offset2 0xfc */
/* @PostProc: swap_instructions --offset1 0x104 --offset2 0x114 */
/* @PostProc: swap_instructions --offset1 0x108 --offset2 0x110 */
/* @PostProc: swap_instructions --offset1 0x10c --offset2 0x118 */
/* @PostProc: swap_instructions --offset1 0x110 --offset2 0x118 */
/* @PostProc: swap_instructions --offset1 0x114 --offset2 0x118 */
/* @PostProc: swap_instructions --offset1 0x178 --offset2 0x180 */
/* @PostProc: swap_instructions --offset1 0x184 --offset2 0x18c */
/* @PostProc: swap_instructions --offset1 0x190 --offset2 0x194 */
/* @PostProc: swap_instructions --offset1 0x198 --offset2 0x19c */
/* @PostProc: swap_instructions --offset1 0x1ac --offset2 0x1b0 */
/* @PostProc: swap_instructions --offset1 0x1b0 --offset2 0x1b4 */
/* @PostProc: swap_instructions --offset1 0x1e4 --offset2 0x1ec */
/* @PostProc: swap_instructions --offset1 0x1e8 --offset2 0x1f0 */
/* @PostProc: swap_instructions --offset1 0x1fc --offset2 0x208 */
/* @PostProc: swap_instructions --offset1 0x200 --offset2 0x204 */
/* @PostProc: swap_instructions --offset1 0x204 --offset2 0x218 */
/* @PostProc: swap_instructions --offset1 0x208 --offset2 0x218 */
/* @PostProc: swap_instructions --offset1 0x20c --offset2 0x218 */
/* @PostProc: swap_instructions --offset1 0x210 --offset2 0x214 */
/* @PostProc: swap_instructions --offset1 0x250 --offset2 0x26c */
/* @PostProc: swap_instructions --offset1 0x254 --offset2 0x268 */
/* @PostProc: swap_instructions --offset1 0x258 --offset2 0x264 */
/* @PostProc: swap_instructions --offset1 0x25c --offset2 0x260 */
#include "types.h"

typedef struct {
    u32 w[8];
} Blk8;

typedef struct {
    s16 tag;
    s16 f02;
    u8  pad[4];
    Blk8 blk;
} Rec40;

typedef struct {
    u32  flags;
    Blk8 a;
    Blk8 b;
    u32  f44;
} Voice;

extern Blk8  GsIDMATRIX;
extern Rec40 GsDSTACK[];

extern s32 get_dstack(s32 *out);
extern s32 mul_dmat(Blk8 *dst, Rec40 *rec);
extern void mul_dmat_light(Blk8 *dst, Rec40 *rec);

void GsGetObjLw(Voice *p)
{
    s32 tmp;
    s32 n;
    s32 i;
    s32 flags;
    Rec40 *r;
    Blk8 *bp;

    tmp = (s32)&p->f44;
    flags = 0;
    n = get_dstack(&tmp);

    p->a = GsIDMATRIX;

    if (n >= 0) {
        bp = &GsDSTACK[n].blk;
        r = &GsDSTACK[n];
        i = n;
        do {
            if (GsDSTACK[i].tag == 128) {
                p->a = *bp;
                flags = GsDSTACK[i].f02;
            } else {
                flags |= mul_dmat(&p->a, r);
            }
            r--;
            bp = (Blk8 *)((char *)bp - 40);
            i--;
        } while (i >= 0);
    }

    if (flags == 1) {
        p->b = GsIDMATRIX;
        if (n >= 0) {
            do {
                mul_dmat_light(&p->b, &GsDSTACK[n]);
                n--;
            } while (n >= 0);
        }
    } else {
        p->b = p->a;
    }

    if (tmp == 1) {
        p->flags |= 0x20000000;
    } else {
        p->flags &= 0xDFFFFFFF;
    }
}
