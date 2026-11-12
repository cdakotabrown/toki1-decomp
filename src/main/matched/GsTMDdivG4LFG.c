/* Address: 0x800AD79C */
/* Size: 792 bytes (198 instructions) as recorded -- REAL SIZE 776 (194). */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @SymbolSize: 776 */
/* @CompileEnv: IDO52_AS1_NO_LA_PULL=all IDO52_FORCE_WEB_COLOR=2:s7,4:s8,31:s1,29:t2,131:t3,132:t4,133:t5,108:s6,118:s5 IDO52_FORCE_WEB_ADJSAVE=8:1,12:1,16:1,29:1 IDO52_AS1_NO_COPY_PROP=all IDO52_AS1_NO_PASS1_PROP=all IDO52_FORCE_WEB_RANGE_SPLIT=61 IDO52_UGEN_TEMP_REG=7:t6,5:t8,10:t6,13:t9,11:t8,16:t9,19:t6,17:t8,25:t6,23:t8 IDO52_AS1_TIE_TAKE=491,387,392,395 IDO52_AS1_TIE_REJECT=501,506 */
/* @CompileFlags: -Wc,-pic1 -Wo,-do_opt_saved_regs -Wo,-noprecolor */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: remap_stack_slots --map '56:72,60:76,64:80,68:84,88:92,92:96,96:100,100:104,104:108,108:112,112:132,116:136,120:140,124:144,152:156,156:160,160:164' */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0xc */
/* @PostProc: swap_instructions --offset1 0x54 --offset2 0x64 */
/* @PostProc: swap_instructions --offset1 0x58 --offset2 0x68 */
/* @PostProc: swap_instructions --offset1 0x5c --offset2 0x6c */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x70 */
/* @PostProc: swap_instructions --offset1 0x64 --offset2 0x74 */
/* @PostProc: swap_instructions --offset1 0x68 --offset2 0x78 */
/* @PostProc: swap_instructions --offset1 0x6c --offset2 0x8c */
/* @PostProc: swap_instructions --offset1 0x70 --offset2 0x90 */
/* @PostProc: swap_instructions --offset1 0x74 --offset2 0x94 */
/* @PostProc: swap_instructions --offset1 0x78 --offset2 0x98 */
/* @PostProc: swap_instructions --offset1 0x7c --offset2 0x94 */
/* @PostProc: swap_instructions --offset1 0x80 --offset2 0x88 */
/* @PostProc: swap_instructions --offset1 0x84 --offset2 0x98 */
/* @PostProc: swap_instructions --offset1 0x88 --offset2 0x98 */
/* @PostProc: swap_instructions --offset1 0x214 --offset2 0x218 */
/* @PostProc: swap_instructions --offset1 0x188 --offset2 0x194 */
/* @PostProc: swap_instructions --offset1 0x194 --offset2 0x1a4 */
/* @PostProc: patch_insn_reg_field 0x168:rd:t9:t6 0x16c:rs:t9:t6 0x170:rs:t9:t6 0x180:rs:t9:t6 0x180:rt:t6:t9 0x184:rs:t9:t6 0x184:rt:t6:t9 0x18c:rt:t6:t9 0x190:rt:t6:t9 */
#include "types.h"

typedef struct { s16 x, y, z, pad; } Vtx8;
typedef union { u32 w; struct { u8 r, g, b, code; } b; } Col4;

typedef struct {
    Vtx8 v;
    u32 f8;
    u32 col;
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
    u16 n;
    u16 i;
} PolyVtx;

typedef struct {
    u8 pad0[3];
    u8 code;
    u32 col;
    PolyVtx vt[4];
} PolyG4;

typedef struct {
    s32 f0;
    s32 *otBase;
    s32 otOfs;
} OtCtx;

extern s32 RotAverageNclip4(Vtx8 *, Vtx8 *, Vtx8 *, Vtx8 *, s32 *, s32 *,
                         s32 *, s32 *, s32 *, s32 *, s32 *);
extern void ReadSZfifo4(s32 *, s32 *, s32 *, s32 *);
extern void NormalColorDpq3(u32 *, u32 *, u32 *, u32 *, s32, u32 *, u32 *, u32 *);
extern void NormalColorDpq(u32 *, u32 *, s32, u32 *);
extern void *RCpolyG4A(void *, Work *, s32, void *);
extern s32 GsTON;

void *GsTMDdivG4LFG(PolyG4 *poly, Vtx8 *vtx, u32 (*nrm)[2], void *out,
                    u32 count, s32 shift, OtCtx *ctx, Work *work)
{
    u32 i;
    s32 otz;
    s32 p;
    s32 flag;
    s32 pad0;
    s32 pad1;
    s32 pad2;
    s32 pad3;
    WorkSlot *p0;
    WorkSlot *p1;
    WorkSlot *p2;
    WorkSlot *p3;

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
        p0->v = vtx[poly->vt[0].i];
        p1->v = vtx[poly->vt[1].i];
        p2->v = vtx[poly->vt[2].i];
        p3->v = vtx[poly->vt[3].i];
        if (RotAverageNclip4(&p0->v, &p1->v, &p2->v, &p3->v, &p0->f16,
                          &p1->f16, &p2->f16, &p3->f16, &p, &otz,
                          &flag) > 0) {
            ReadSZfifo4(&p0->f20, &p1->f20, &p2->f20, &p3->f20);
            NormalColorDpq3(nrm[poly->vt[0].n], nrm[poly->vt[1].n],
                          nrm[poly->vt[2].n], &poly->col, p,
                          &p0->col, &p1->col, &p2->col);
            NormalColorDpq(nrm[poly->vt[3].n], &poly->col, p, &p3->col);
            work->ot = ctx->otBase +
                       ((u32)((u16)otz - ctx->otOfs) >> shift);
            work->col.b.code = (u8)((GsTON << 1) | poly->code);
            out = RCpolyG4A(out, work, 0, work->tail);
        }
        poly++;
        i++;
    } while (i < count);
    }
    return out;
}
