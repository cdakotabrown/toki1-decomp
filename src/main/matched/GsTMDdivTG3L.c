/* Address: 0x800AC8B0 */
/* Size: 684 bytes (171 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @CompileEnv: IDO52_AS1_NO_FALLTHRU_FILL=all IDO52_AS1_NO_LA_PULL=all IDO52_FORCE_WEB_COLOR=5:s2,7:s3,11:s4,15:s5,20:s6,27:s7,43:s8,125:t1 IDO52_FORCE_WEB_NOSPILL=122,123,124 IDO52_UGEN_TEMP_REG=10:t5,8:t9,17:t2,18:t3,19:t5,22:t4,20:t8,26:t8,29:t4 */
/* @CompileFlags: -Wc,-pic1 -Wo,-do_opt_saved_regs */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: remap_stack_slots --map '68:92,72:96,76:100,80:104,84:108,88:112,92:116,96:120,100:124,124:128,128:132,132:136,136:140,140:144,144:148,148:152' */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0x14 */
/* @PostProc: swap_instructions --offset1 0xc --offset2 0x10 */
/* @PostProc: swap_instructions --offset1 0x10 --offset2 0x18 */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x24 */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x20 */
/* @PostProc: swap_instructions --offset1 0x1c --offset2 0x24 */
/* @PostProc: swap_instructions --offset1 0x20 --offset2 0x2c */
/* @PostProc: swap_instructions --offset1 0x24 --offset2 0x2c */
/* @PostProc: swap_instructions --offset1 0x28 --offset2 0x2c */
/* @PostProc: swap_instructions --offset1 0x6c --offset2 0x84 */
/* @PostProc: swap_instructions --offset1 0x70 --offset2 0x84 */
/* @PostProc: swap_instructions --offset1 0x74 --offset2 0x9c */
/* @PostProc: swap_instructions --offset1 0x78 --offset2 0x84 */
/* @PostProc: swap_instructions --offset1 0x7c --offset2 0x98 */
/* @PostProc: swap_instructions --offset1 0x80 --offset2 0x9c */
/* @PostProc: swap_instructions --offset1 0x84 --offset2 0x94 */
/* @PostProc: swap_instructions --offset1 0x88 --offset2 0x94 */
/* @PostProc: swap_instructions --offset1 0x8c --offset2 0x90 */
/* @PostProc: swap_instructions --offset1 0x90 --offset2 0x98 */
/* @PostProc: swap_instructions --offset1 0x94 --offset2 0x9c */
/* @PostProc: swap_instructions --offset1 0x98 --offset2 0x9c */
/* @PostProc: swap_instructions --offset1 0x168 --offset2 0x19c */
/* @PostProc: swap_instructions --offset1 0x16c --offset2 0x1a0 */
/* @PostProc: swap_instructions --offset1 0x170 --offset2 0x1a4 */
/* @PostProc: swap_instructions --offset1 0x174 --offset2 0x1a8 */
/* @PostProc: swap_instructions --offset1 0x178 --offset2 0x19c */
/* @PostProc: swap_instructions --offset1 0x17c --offset2 0x1a0 */
/* @PostProc: swap_instructions --offset1 0x180 --offset2 0x1a4 */
/* @PostProc: swap_instructions --offset1 0x184 --offset2 0x1a8 */
/* @PostProc: swap_instructions --offset1 0x188 --offset2 0x1ac */
/* @PostProc: swap_instructions --offset1 0x18c --offset2 0x1b4 */
/* @PostProc: swap_instructions --offset1 0x190 --offset2 0x19c */
/* @PostProc: swap_instructions --offset1 0x194 --offset2 0x1a0 */
/* @PostProc: swap_instructions --offset1 0x19c --offset2 0x1a4 */
/* @PostProc: swap_instructions --offset1 0x1a0 --offset2 0x1a8 */
/* @PostProc: swap_instructions --offset1 0x1a4 --offset2 0x1ac */
/* @PostProc: swap_instructions --offset1 0x1a8 --offset2 0x1b4 */
#include "types.h"

typedef struct { s16 x, y, z, pad; } Vtx8;

typedef struct {
    Vtx8 v;
    u32 f8;
    s32 col;
    s32 f16;
    s32 f20;
} WorkSlot;

typedef struct {
    u8 pad0[12];
    u16 f12;
    u16 f14;
    u8 pad16[3];
    u8 code;
    s32 *ot;
    WorkSlot slot[3];
    u8 tail[72];
    WorkSlot *ptr[3];
} Work;

typedef struct {
    u16 n;
    u16 i;
} PolyVtx;

typedef union { u32 w; struct { u16 lo, hi; } h; } UV;

typedef struct {
    u8 pad0[3];
    u8 code;
    UV uv[3];
    PolyVtx vt[3];
} PolyGT3N;

typedef struct {
    s32 f0;
    s32 *otBase;
    s32 otOfs;
} OtCtx;

extern s32 RotAverageNclipColorCol3(Vtx8 *, Vtx8 *, Vtx8 *, s16 *, s16 *, s16 *,
                         s32 *, s32 *, s32 *, s32 *, s32 *, s32 *, s32 *,
                         s32 *, s32 *);
extern void ReadSZfifo3(s32 *, s32 *, s32 *);
extern void *RCpolyGT3A(void *, Work *, s32, void *);
extern s32 GsTON;

void *GsTMDdivTG3L(PolyGT3N *poly, Vtx8 *vtx, s16 (*nrm)[4], void *out,
                    u32 count, s32 shift, OtCtx *ctx, Work *work)
{
    s32 unused;
    s32 otz;
    s32 p;
    s32 flag;
    s32 col;
    s32 pad0;
    s32 pad1;
    s32 pad2;
    WorkSlot *p0;
    WorkSlot *p1;
    WorkSlot *p2;
    u32 i;

    col = 0x34808080;
    p0 = &work->slot[0];
    p1 = &work->slot[1];
    p2 = &work->slot[2];
    work->ptr[0] = p0;
    work->ptr[1] = p1;
    work->ptr[2] = p2;

    i = 0;
    if (count != 0) {
    do {
        p0->v = vtx[poly->vt[0].i];
        p1->v = vtx[poly->vt[1].i];
        p2->v = vtx[poly->vt[2].i];
        if (RotAverageNclipColorCol3(&p0->v, &p1->v, &p2->v, nrm[poly->vt[0].n],
                          nrm[poly->vt[1].n], nrm[poly->vt[2].n],
                          &col, &p0->f16, &p1->f16, &p2->f16,
                          &p0->col, &p1->col, &p2->col, &otz, &flag) > 0) {
            ReadSZfifo3(&p0->f20, &p1->f20, &p2->f20);
            work->ot = ctx->otBase +
                       ((u32)((u16)otz - ctx->otOfs) >> shift);
            work->code = (u8)((GsTON << 1) | poly->code);
            work->f12 = poly->uv[0].h.hi;
            work->f14 = poly->uv[1].h.hi;
            p0->f8 = poly->uv[0].w;
            p1->f8 = poly->uv[1].w;
            p2->f8 = poly->uv[2].w;
            out = RCpolyGT3A(out, work, 0, work->tail);
        }
        poly++;
        i++;
    } while (i < count);
    }
    return out;
}
