/* Address: 0x800AF7FC */
/* Size: 732 bytes (183 instructions) as recorded -- REAL SIZE 716 (179). */
/* @SymbolSize: 716 */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @CompileEnv: IDO52_AS1_NO_FALLTHRU_FILL=all IDO52_AS1_NO_LA_PULL=all IDO52_FORCE_WEB_COLOR=2:s2,4:s3,8:s4,12:s5,16:s6,29:s7,22:s8 IDO52_FORCE_WEB_NOSPILL=136,137 IDO52_UGEN_TEMP_REG=7:t2,5:t6,14:t7,15:t8,16:t2,27:t8,19:t9,29:t2,17:t5,32:t5,33:t6 */
/* @CompileFlags: -Wc,-pic1 -Wo,-do_opt_saved_regs */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: remap_stack_slots --map '52:68,56:72,60:76,64:80,68:84,72:88,76:92,80:96,84:100,108:104,112:108,116:112,120:116,124:120' */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0xc */
/* @PostProc: swap_instructions --offset1 0xc --offset2 0x14 */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x18 */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x20 */
/* @PostProc: swap_instructions --offset1 0x1c --offset2 0x20 */
/* @PostProc: swap_instructions --offset1 0x68 --offset2 0x78 */
/* @PostProc: swap_instructions --offset1 0x6c --offset2 0x78 */
/* @PostProc: swap_instructions --offset1 0x70 --offset2 0x88 */
/* @PostProc: swap_instructions --offset1 0x74 --offset2 0x78 */
/* @PostProc: swap_instructions --offset1 0x78 --offset2 0x84 */
/* @PostProc: swap_instructions --offset1 0x7c --offset2 0x88 */
/* @PostProc: swap_instructions --offset1 0x80 --offset2 0x84 */
/* @PostProc: swap_instructions --offset1 0x84 --offset2 0x88 */
/* @PostProc: patch_insn_reg_field 0x130:rt:t2:t7 0x17c:rt:t2:t7 */
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

typedef union { u32 w; struct { u16 lo, hi; } h; } UV;

typedef struct {
    u8 pad0[3];
    u8 code;
    UV uv[4];
    u32 col[4];
    u16 i0;
    u16 i1;
    u16 i2;
    u16 i3;
} PolyGT4;

typedef struct {
    s32 f0;
    s32 *otBase;
    s32 otOfs;
} OtCtx;

extern s32 RotAverageNclip4(Vtx8 *, Vtx8 *, Vtx8 *, Vtx8 *, s32 *, s32 *,
                         s32 *, s32 *, s32 *, s32 *, s32 *);
extern void ReadSZfifo4(s32 *, s32 *, s32 *, s32 *);
extern void *RCpolyGT4A(void *, Work *, s32, void *);
extern s32 GsTON;

void *GsTMDdivTNG4(PolyGT4 *poly, Vtx8 *vtx, void *out,
                    u32 count, s32 shift, OtCtx *ctx, Work *work)
{
    s32 unused;
    s32 otz;
    s32 p;
    s32 flag;
    s32 pad0;
    s32 pad1;
    WorkSlot *p0;
    WorkSlot *p1;
    WorkSlot *p2;
    WorkSlot *p3;
    u32 i;

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
        if (RotAverageNclip4(&p0->v, &p1->v, &p2->v, &p3->v, &p0->f16,
                          &p1->f16, &p2->f16, &p3->f16, &p, &otz,
                          &flag) > 0) {
            ReadSZfifo4(&p0->f20, &p1->f20, &p2->f20, &p3->f20);
            work->ot = ctx->otBase +
                       ((u32)((u16)otz - ctx->otOfs) >> shift);
            work->col.b.code = (u8)((GsTON << 1) | (poly->code & 0xFE));
            work->f12 = poly->uv[0].h.hi;
            work->f14 = poly->uv[1].h.hi;
            p0->col = poly->col[0];
            p1->col = poly->col[1];
            p2->col = poly->col[2];
            p3->col = poly->col[3];
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
