/* Address: 0x800AE47C */
/* Size: 720 bytes (180 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @CompileFlags: -Wc,-pic1 -Wo,-do_opt_saved_regs */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: remap_stack_slots --map '56:72,60:76,64:80,68:84,72:88,76:92,80:96,84:100,112:108,108:104,116:112,120:116,124:120,128:124' */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0x14 */
/* @PostProc: swap_instructions --offset1 0xc --offset2 0x10 */
/* @PostProc: swap_instructions --offset1 0x10 --offset2 0x1c */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x18 */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x28 */
/* @PostProc: swap_instructions --offset1 0x1c --offset2 0x24 */
/* @PostProc: swap_instructions --offset1 0x20 --offset2 0x28 */
/* @PostProc: swap_instructions --offset1 0x24 --offset2 0x2c */
/* @PostProc: swap_instructions --offset1 0x28 --offset2 0x2c */
/* @PostProc: swap_instructions --offset1 0x2c --offset2 0x30 */
/* @PostProc: swap_instructions --offset1 0x70 --offset2 0x80 */
/* @PostProc: swap_instructions --offset1 0x74 --offset2 0x84 */
/* @PostProc: swap_instructions --offset1 0x78 --offset2 0x80 */
/* @PostProc: swap_instructions --offset1 0x7c --offset2 0x94 */
/* @PostProc: swap_instructions --offset1 0x80 --offset2 0x84 */
/* @PostProc: swap_instructions --offset1 0x84 --offset2 0x90 */
/* @PostProc: swap_instructions --offset1 0x88 --offset2 0x90 */
/* @PostProc: swap_instructions --offset1 0x8c --offset2 0x94 */
/* @PostProc: swap_instructions --offset1 0x1b8 --offset2 0x1c0 */
/* @PostProc: patch_insn_reg_field 0x154:rd:t6:t7 0x158:rs:t6:t7 0x15c:rs:t6:t7 0x16c:rs:t6:t7 0x16c:rt:t7:t6 0x170:rs:t6:t7 0x170:rt:t7:t6 0x178:rt:t7:t6 0x17c:rt:t7:t6 */
/* @CompileEnv: IDO52_AS1_NO_FALLTHRU_FILL=all IDO52_AS1_NO_LA_PULL=all IDO52_FORCE_WEB_COLOR=5:s2,7:s4,11:s5,15:s6,19:s7,32:s8 IDO52_FORCE_WEB_NOSPILL=135 IDO52_UGEN_TEMP_REG=8:t3,6:t7,15:t8,16:t9,17:t3,28:t9,20:t2,18:t7,27:t8,29:t2 */
#include "types.h"

typedef struct { s16 x, y, z, pad; } Vtx8;
typedef union { u32 w; u8 b[4]; } Col4;
typedef union { u32 w; struct { u16 lo, hi; } h; } UV;

typedef struct {
    Vtx8 v;
    u32 f8;
    s32 f12;
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
    u16 tex;
    u16 idx[4];
    u16 pad30;
} PolyGT4;

typedef struct {
    s32 f0;
    s32 *otBase;
    s32 otOfs;
} OtCtx;

extern s32 RotAverageNclip4(Vtx8 *, Vtx8 *, Vtx8 *, Vtx8 *,
                         s32 *, s32 *, s32 *, s32 *,
                         s32 *, s32 *, s32 *);
extern void ReadSZfifo4(s32 *, s32 *, s32 *, s32 *);
extern void NormalColorDpq(u32 *, Col4 *, s32, Col4 *);
extern void *RCpolyFT4A(void *, Work *, s32, void *);
extern s32 GsTON;

void *GsTMDdivTF4LFG(PolyGT4 *poly, Vtx8 *vtx, u32 (*texBase)[2], void *out,
                    u32 count, s32 shift, OtCtx *ctx, Work *work)
{
    u32 i;
    s32 otz;
    s32 p;
    s32 flag;
    Col4 defCol;
    s32 padLo[2];
    WorkSlot *p0;
    WorkSlot *p1;
    WorkSlot *p2;
    WorkSlot *p3;

    defCol.w = 0x2C808080;
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
        p0->v = vtx[poly->idx[0]];
        p1->v = vtx[poly->idx[1]];
        p2->v = vtx[poly->idx[2]];
        p3->v = vtx[poly->idx[3]];
        if (RotAverageNclip4(&p0->v, &p1->v, &p2->v, &p3->v,
                          &p0->f16, &p1->f16, &p2->f16, &p3->f16,
                          &p, &otz, &flag) > 0) {
            ReadSZfifo4(&p0->f20, &p1->f20, &p2->f20, &p3->f20);
            NormalColorDpq(texBase[poly->tex], &defCol, p, &work->col);
            work->ot = ctx->otBase +
                       ((u32)((u16)otz - ctx->otOfs) >> shift);
            work->col.b[3] = (u8)((GsTON << 1) | poly->code);
            work->f12 = poly->uv[0].h.hi;
            work->f14 = poly->uv[1].h.hi;
            p0->f8 = poly->uv[0].w;
            p1->f8 = poly->uv[1].w;
            p2->f8 = poly->uv[2].w;
            p3->f8 = poly->uv[3].w;
            out = RCpolyFT4A(out, work, 0, work->tail);
        }
        poly++;
        i++;
    } while (i < count);
    }
    return out;
}
