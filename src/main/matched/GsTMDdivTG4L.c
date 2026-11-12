/* Address: 0x800AEEC0 */
/* Size: 824 bytes (206 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @CompileFlags: -Wc,-pic1 -Wo,-do_opt_saved_regs -Wo,-noprecolor */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: remap_stack_slots --map '56:80,60:84,64:88,68:92,72:96,76:100,96:108,100:112,104:116,108:120,112:140,116:144,120:148,124:152,148:160,144:156' */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0x10 */
/* @PostProc: swap_instructions --offset1 0xc --offset2 0x18 */
/* @PostProc: swap_instructions --offset1 0x10 --offset2 0x14 */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x20 */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x20 */
/* @PostProc: swap_instructions --offset1 0x1c --offset2 0x24 */
/* @PostProc: swap_instructions --offset1 0x24 --offset2 0x28 */
/* @PostProc: swap_instructions --offset1 0x68 --offset2 0x78 */
/* @PostProc: swap_instructions --offset1 0x6c --offset2 0x7c */
/* @PostProc: swap_instructions --offset1 0x70 --offset2 0x80 */
/* @PostProc: swap_instructions --offset1 0x74 --offset2 0x84 */
/* @PostProc: swap_instructions --offset1 0x78 --offset2 0x98 */
/* @PostProc: swap_instructions --offset1 0x7c --offset2 0x9c */
/* @PostProc: swap_instructions --offset1 0x80 --offset2 0x98 */
/* @PostProc: swap_instructions --offset1 0x84 --offset2 0x94 */
/* @PostProc: swap_instructions --offset1 0x88 --offset2 0x9c */
/* @PostProc: swap_instructions --offset1 0x8c --offset2 0x90 */
/* @PostProc: swap_instructions --offset1 0x90 --offset2 0x98 */
/* @PostProc: swap_instructions --offset1 0x98 --offset2 0x9c */
/* @PostProc: patch_insn_reg_field 0x160:rd:t5:t8 0x164:rt:t5:t8 0x164:rd:t4:t5 0x168:rs:t4:t5 0x16c:rs:t4:t5 0x17c:rs:t4:t5 0x17c:rt:t5:t4 0x180:rs:t4:t5 0x180:rt:t5:t4 0x188:rt:t5:t4 0x18c:rt:t5:t4 */
/* @CompileEnv: IDO52_AS1_NO_PASS1_PROP=all IDO52_AS1_NO_COPY_PROP=2 IDO52_AS1_NO_LA_PULL=all IDO52_FORCE_WEB_COLOR=5:s4,7:s6,11:s7,15:s8,135:s5,32:t0,148:t1,149:t2,150:t3 IDO52_FORCE_WEB_SPILL=121 IDO52_FORCE_WEB_RANGE_SPLIT=64 IDO52_FORCE_WEB_ADJSAVE=19:0.5,32:0.4 IDO52_UGEN_TEMP_REG=8:t5,6:t9,14:t9,16:t4,20:t7,28:t6,26:t5 */
#include "types.h"

typedef struct { s16 x, y, z, pad; } Vtx8;
typedef union { u32 w; u8 b[4]; } Col4;
typedef union { u32 w; struct { u16 lo, hi; } h; } UV;

typedef struct {
    Vtx8 v;
    u32 f8;
    Col4 f12;
    s32 f16;
    s32 f20;
} WorkSlot;

typedef struct {
    u8 pad0[12];
    u16 f12;
    u16 f14;
    Col4 col;
    s32 *ot;
    WorkSlot slot[4];
    u8 tail[120];
    WorkSlot *ptr[4];
} Work;

typedef struct {
    u8 pad0[3];
    u8 code;
    UV uv[4];
    u16 n0;
    u16 i0;
    u16 n1;
    u16 i1;
    u16 n2;
    u16 i2;
    u16 n3;
    u16 i3;
} PolyGT4N;

typedef struct {
    s32 f0;
    s32 *otBase;
    s32 otOfs;
} OtCtx;

extern s32 RotAverageNclip4(Vtx8 *, Vtx8 *, Vtx8 *, Vtx8 *,
                         s32 *, s32 *, s32 *, s32 *,
                         s32 *, s32 *, s32 *);
extern void ReadSZfifo4(s32 *, s32 *, s32 *, s32 *);
extern void NormalColorCol3(u32 *, u32 *, u32 *, Col4 *,
                          Col4 *, Col4 *, Col4 *);
extern void NormalColorCol(u32 *, Col4 *, Col4 *);
extern void *RCpolyGT4A(void *, Work *, s32, void *);
extern s32 GsTON;

void *GsTMDdivTG4L(PolyGT4N *poly, Vtx8 *vtx, u32 (*nrm)[2], void *out,
                    u32 count, s32 shift, OtCtx *ctx, Work *work)
{
    u32 i;
    s32 otz;
    s32 p;
    s32 flag;
    Col4 defCol;
    s32 padLo[6];
    WorkSlot *p0;
    WorkSlot *p1;
    WorkSlot *p2;
    WorkSlot *p3;

    defCol.w = 0x3C808080;
    p0 = &work->slot[0];
    p1 = &work->slot[1];
    p2 = &work->slot[2];
    p3 = &work->slot[3];
    work->ptr[0] = p0;
    work->ptr[1] = p1;
    work->ptr[2] = p2;
    work->ptr[3] = p3;

    i = 0;
    if (count != 0) {
    do {
        p0->v = vtx[poly->i0];
        p1->v = vtx[poly->i1];
        p2->v = vtx[poly->i2];
        p3->v = vtx[poly->i3];
        if (RotAverageNclip4(&p0->v, &p1->v, &p2->v, &p3->v,
                          &p0->f16, &p1->f16, &p2->f16, &p3->f16,
                          &p, &otz, &flag) > 0) {
            ReadSZfifo4(&p0->f20, &p1->f20, &p2->f20, &p3->f20);
            NormalColorCol3(nrm[poly->n0], nrm[poly->n1], nrm[poly->n2],
                          &defCol, &p0->f12, &p1->f12, &p2->f12);
            NormalColorCol(nrm[poly->n3], &defCol, &p3->f12);
            work->ot = ctx->otBase +
                       ((u32)((u16)otz - ctx->otOfs) >> shift);
            work->col.b[3] = (u8)(((GsTON << 1) | poly->code) & 0xFE);
            work->f12 = poly->uv[0].h.hi;
            work->f14 = poly->uv[1].h.hi;
            p0->f8 = poly->uv[0].w;
            p1->f8 = poly->uv[1].w;
            p2->f8 = poly->uv[2].w;
            p3->f8 = poly->uv[3].w;
            out = RCpolyGT4A(out, work, 0, work->tail);
        }
        poly++;
        i++;
    } while (i < count);
    }
    return out;
}
