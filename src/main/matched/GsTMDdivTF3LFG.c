/* @CompileFlags: -Wc,-pic1 -Wo,-do_opt_saved_regs */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: remap_stack_slots --map '72:104,76:108,80:112,84:116,88:120,92:124,132:148,128:144,124:140,120:136,172:184,144:180,140:176,136:172,152:168,148:164' */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0x10 */
/* @PostProc: swap_instructions --offset1 0xc --offset2 0x18 */
/* @PostProc: swap_instructions --offset1 0x10 --offset2 0x14 */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x20 */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x20 */
/* @PostProc: swap_instructions --offset1 0x1c --offset2 0x24 */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x78 */
/* @PostProc: swap_instructions --offset1 0x64 --offset2 0x7c */
/* @PostProc: swap_instructions --offset1 0x68 --offset2 0x80 */
/* @PostProc: swap_instructions --offset1 0x6c --offset2 0x84 */
/* @PostProc: swap_instructions --offset1 0x70 --offset2 0xa0 */
/* @PostProc: swap_instructions --offset1 0x74 --offset2 0x78 */
/* @PostProc: swap_instructions --offset1 0x78 --offset2 0x9c */
/* @PostProc: swap_instructions --offset1 0x7c --offset2 0x84 */
/* @PostProc: swap_instructions --offset1 0x80 --offset2 0x90 */
/* @PostProc: swap_instructions --offset1 0x84 --offset2 0xa0 */
/* @PostProc: swap_instructions --offset1 0x88 --offset2 0x8c */
/* @PostProc: swap_instructions --offset1 0x8c --offset2 0x9c */
/* @PostProc: swap_instructions --offset1 0x90 --offset2 0x9c */
/* @PostProc: swap_instructions --offset1 0x94 --offset2 0xa0 */
/* @PostProc: swap_instructions --offset1 0x98 --offset2 0x9c */
/* @PostProc: swap_instructions --offset1 0x9c --offset2 0xa0 */
/* @PostProc: swap_instructions --offset1 0x164 --offset2 0x1ac */
/* @PostProc: swap_instructions --offset1 0x168 --offset2 0x1ac */
/* @PostProc: swap_instructions --offset1 0x16c --offset2 0x1ac */
/* @PostProc: swap_instructions --offset1 0x170 --offset2 0x1ac */
/* @PostProc: swap_instructions --offset1 0x174 --offset2 0x1a4 */
/* @PostProc: swap_instructions --offset1 0x178 --offset2 0x1a8 */
/* @PostProc: swap_instructions --offset1 0x17c --offset2 0x1ac */
/* @PostProc: swap_instructions --offset1 0x180 --offset2 0x1a4 */
/* @PostProc: swap_instructions --offset1 0x184 --offset2 0x1bc */
/* @PostProc: swap_instructions --offset1 0x188 --offset2 0x1c0 */
/* @PostProc: swap_instructions --offset1 0x18c --offset2 0x1a8 */
/* @PostProc: swap_instructions --offset1 0x190 --offset2 0x1ac */
/* @PostProc: swap_instructions --offset1 0x194 --offset2 0x1c4 */
/* @PostProc: swap_instructions --offset1 0x198 --offset2 0x1c8 */
/* @PostProc: swap_instructions --offset1 0x19c --offset2 0x1a4 */
/* @PostProc: swap_instructions --offset1 0x1a0 --offset2 0x1bc */
/* @PostProc: swap_instructions --offset1 0x1a4 --offset2 0x1cc */
/* @PostProc: swap_instructions --offset1 0x1a8 --offset2 0x1d4 */
/* @PostProc: swap_instructions --offset1 0x1ac --offset2 0x1c0 */
/* @PostProc: swap_instructions --offset1 0x1b0 --offset2 0x1d4 */
/* @PostProc: swap_instructions --offset1 0x1b4 --offset2 0x1c0 */
/* @PostProc: swap_instructions --offset1 0x1b8 --offset2 0x1c4 */
/* @PostProc: swap_instructions --offset1 0x1bc --offset2 0x1c8 */
/* @PostProc: swap_instructions --offset1 0x1c0 --offset2 0x1cc */
/* @PostProc: swap_instructions --offset1 0x1c4 --offset2 0x1c8 */
/* @PostProc: swap_instructions --offset1 0x1c8 --offset2 0x1d4 */
/* @PostProc: swap_instructions --offset1 0x28c --offset2 0x2bc */
/* @PostProc: swap_instructions --offset1 0x290 --offset2 0x2bc */
/* @PostProc: swap_instructions --offset1 0x294 --offset2 0x2bc */
/* @PostProc: swap_instructions --offset1 0x298 --offset2 0x2bc */
/* @PostProc: swap_instructions --offset1 0x29c --offset2 0x2bc */
/* @PostProc: swap_instructions --offset1 0x2a0 --offset2 0x2bc */
/* @PostProc: swap_instructions --offset1 0x2a4 --offset2 0x2bc */
/* @PostProc: swap_instructions --offset1 0x2a8 --offset2 0x2bc */
/* @PostProc: swap_instructions --offset1 0x2ac --offset2 0x2bc */
/* @PostProc: swap_instructions --offset1 0x2b0 --offset2 0x2bc */
/* @PostProc: swap_instructions --offset1 0x2b4 --offset2 0x2bc */
/* @PostProc: swap_instructions --offset1 0x2b8 --offset2 0x2bc */
/* @PostProc: swap_instructions --offset1 0x2ec --offset2 0x2fc */
/* @PostProc: swap_instructions --offset1 0x2f0 --offset2 0x300 */
/* @PostProc: swap_instructions --offset1 0x2f4 --offset2 0x304 */
/* @PostProc: swap_instructions --offset1 0x2f8 --offset2 0x2fc */
/* @PostProc: swap_instructions --offset1 0x2fc --offset2 0x300 */
/* @PostProc: swap_instructions --offset1 0x300 --offset2 0x304 */
/* @PostProc: patch_insn_reg_field 0x1f4:rt:t5:v0 0x1fc:rs:t5:v0 0x1fc:rt:t8:t5 0x204:rt:t8:t5 0x208:rs:t5:v0 0x214:rs:t9:t3 0x214:rt:t3:t9 0x340:rs:t9:t1 0x340:rt:t1:t9 */
/* @CompileEnv: IDO52_AS1_NO_FALLTHRU_FILL=all IDO52_AS1_NO_LA_PULL=all IDO52_FORCE_WEB_COLOR=7:s8,11:s6,15:s7,52:v1,27:t0 IDO52_FORCE_WEB_RANGE_SPLIT=69 IDO52_UGEN_TEMP_REG=4:t3,5:t4,14:t8,15:t9,16:t1,17:t2,20:t3,21:t4,18:t8,26:t8,27:t7,24:t2,50:t2,31:t2,34:t8,44:t8,52:t5,56:t3,84:t1 */
#include "types.h"

typedef struct { s16 x, y, z, pad; } Vtx8;
typedef union { u32 w; struct { u16 lo, hi; } h; } UV;

typedef struct {
    Vtx8 v;
    u32  f8;
    s32  f12;
    s32  f16;
    s32  f20;
} WorkSlot;

typedef struct {
    u8 pad0[12];
    u16 f12;
    u16 f14;
    s32 col;
    s32 *ot;
    WorkSlot slot[3];
    u8 tail[72];
    WorkSlot *ptr[3];
} Work;

typedef struct {
    u8 pad0[2];
    u8 flag;
    u8 code;
    UV  uv0;
    UV  uv1;
    u32 col2;
    u16 idx3;
    u16 idx0;
    u16 idx1;
    u16 idx2;
} PolyRec;

typedef struct {
    s32 f0;
    s32 *otBase;
    s32 otOfs;
} OtCtx;

extern s32 RotAverageNclipColorDpq3(Vtx8 *, Vtx8 *, Vtx8 *, s16 *, s16 *, s16 *,
                         s32 *, s32 *, s32 *, s32 *, s32 *, s32 *, s32 *,
                         void *, s32 *);
extern s32 RotAverage3(Vtx8 *, Vtx8 *, Vtx8 *, s32 *, s32 *, s32 *,
                         s32 *, s32 *);
extern void NormalColorDpq3(s16 *, s16 *, s16 *, s32 *, s32, s32 *, s32 *,
                          s32 *);
extern void ReadSZfifo3(s32 *, s32 *, s32 *);
extern void *RCpolyFT3A(void *, Work *, s32, void *);
extern s32 GsTON;

void *GsTMDdivTF3LFG(PolyRec *poly, Vtx8 *vtx, s16 (*aux)[4], void *out,
                    u32 count, s32 shift, OtCtx *ctx, Work *work)
{
    volatile u32 i;
    u16 otz[2];
    s32 p;
    s32 flag;
    s32 hdr;
    WorkSlot *p0;
    WorkSlot *p1;
    WorkSlot *p2;
    s32 pad[7];

    hdr = 0x24808080;
    p0 = &work->slot[0];
    p1 = &work->slot[1];
    p2 = &work->slot[2];
    work->ptr[0] = p0;
    work->ptr[1] = p1;
    work->ptr[2] = p2;

    i = 0;
    if (count != 0) {
    do {
        p0->v = vtx[poly->idx0];
        p1->v = vtx[poly->idx1];
        p2->v = vtx[poly->idx2];
        if ((poly->flag & 2) == 0) {
            if (RotAverageNclipColorDpq3(&p0->v, &p1->v, &p2->v, aux[poly->idx3],
                              aux[poly->idx3], aux[poly->idx3],
                              &hdr, &p0->f16, &p1->f16, &p2->f16,
                              &work->col, &p1->f12, &p2->f12,
                              otz, &flag) > 0) {
                ReadSZfifo3(&p0->f20, &p1->f20, &p2->f20);
                work->ot = ctx->otBase +
                           ((u32)(otz[0] - ctx->otOfs) >> shift);
                ((u8 *)&work->col)[3] =
                    (u8)((GsTON << 1) | poly->code);
                work->f12 = poly->uv0.h.hi;
                work->f14 = poly->uv1.h.hi;
                p0->f8 = poly->uv0.w;
                p1->f8 = poly->uv1.w;
                p2->f8 = poly->col2;
                out = RCpolyFT3A(out, work, 0, work->tail);
            }
        } else {
            otz[0] = RotAverage3(&p0->v, &p1->v, &p2->v,
                                &p0->f16, &p1->f16, &p2->f16, &p, &flag);
            NormalColorDpq3(aux[poly->idx3], aux[poly->idx3], aux[poly->idx3],
                          &hdr, p, &work->col,
                          &p1->f12, &p2->f12);
            ReadSZfifo3(&p0->f20, &p1->f20, &p2->f20);
            work->ot = ctx->otBase +
                       ((u32)(otz[0] - ctx->otOfs) >> shift);
            ((u8 *)&work->col)[3] = (u8)((GsTON << 1) | poly->code);
            work->f12 = poly->uv0.h.hi;
            work->f14 = poly->uv1.h.hi;
            p0->f8 = poly->uv0.w;
            p1->f8 = poly->uv1.w;
            p2->f8 = poly->col2;
            out = RCpolyFT3A(out, work, 0, work->tail);
        }
        poly++;
        i += 1;
    } while (i < count);
    }
    return out;
}
