/* Address: 0x800ABFD8 */
/* Size: 908 bytes (227 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 (t6-t9 temps, addu-form moves, */
/* @CompileFlags: -Wc,-pic1 -Wo,-do_opt_saved_regs */
/* @CompileEnv: IDO52_AS1_NO_FALLTHRU_FILL=all IDO52_AS1_NO_LA_PULL=all IDO52_FORCE_WEB_COLOR=4:s6,8:s7,12:s8,92:s5,24:v1 IDO52_FORCE_WEB_RANGE_SPLIT=55 IDO52_SPLIT_BLOCKS=24#1:0+1+2+3+9 IDO52_FORCE_WEB_NOSPILL=24.0:1e21,55.0:5 IDO52_AS1_NO_BOPT=occ:0 IDO52_UGEN_TEMP_REG=10:t3,11:t3,8:t5,16:t7,14:t4,24:t6,26:t5,28:t0 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: remap_stack_slots --map '48:72,52:76,56:80,60:84,64:88,68:92,104:112,100:108,96:104,92:100,116:136,112:132,108:128,184:196,180:192,176:188,172:184' */
/* @PostProc: patch_insn_reg_field 0xb4:rt:t3:t4 0xbc:rt:t3:t4 0xbc:rd:t4:t5 0xc0:rt:t4:t5 0x110:rt:t2:t1 0x114:rt:t2:t1 0x11c:rt:t2:t1 0x120:rt:t2:t1 0x12c:rt:t6:t2 0x130:rs:t6:t2 0x148:rt:t0:t6 0x154:rt:t0:t6 0x1a4:rd:t2:t0 0x1ac:rt:t2:t0 0x1ac:rd:t6:t2 0x1b0:rt:t6:t2 0x1b4:rt:t0:t6 0x1b8:rs:t0:t6 0x1b8:rt:t0:t6 0x1c4:rt:t0:t6 0x1d8:rd:t2:t0 0x1dc:rt:t2:t0 0x1ec:rt:t6:t2 0x1f4:rt:t6:t2 0x230:rt:t0:t6 0x26c:rt:t0:t6 0x290:rt:t2:t0 0x29c:rs:t2:t0 0x29c:rd:t6:t2 0x2a0:rt:t0:t6 0x2a4:rt:t6:t2 0x2b4:rs:t0:t6 0x2cc:rd:t2:t0 0x2d0:rs:t2:t0 0x2d0:rd:t6:t2 0x2d4:rt:t6:t2 0x2f0:rt:t0:t6 0x2f8:rt:t0:t6 0x334:rt:t2:t0 0x33c:rs:t2:t0 */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0xc */
/* @PostProc: swap_instructions --offset1 0xc --offset2 0x14 */
/* @PostProc: swap_instructions --offset1 0x54 --offset2 0x60 */
/* @PostProc: swap_instructions --offset1 0x58 --offset2 0x64 */
/* @PostProc: swap_instructions --offset1 0x5c --offset2 0x68 */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x6c */
/* @PostProc: swap_instructions --offset1 0x64 --offset2 0x6c */
/* @PostProc: swap_instructions --offset1 0x68 --offset2 0x78 */
/* @PostProc: swap_instructions --offset1 0x70 --offset2 0x78 */
/* @PostProc: swap_instructions --offset1 0x74 --offset2 0x78 */
/* @PostProc: swap_instructions --offset1 0x138 --offset2 0x168 */
/* @PostProc: swap_instructions --offset1 0x13c --offset2 0x168 */
/* @PostProc: swap_instructions --offset1 0x140 --offset2 0x168 */
/* @PostProc: swap_instructions --offset1 0x144 --offset2 0x16c */
/* @PostProc: swap_instructions --offset1 0x148 --offset2 0x174 */
/* @PostProc: swap_instructions --offset1 0x14c --offset2 0x168 */
/* @PostProc: swap_instructions --offset1 0x150 --offset2 0x16c */
/* @PostProc: swap_instructions --offset1 0x154 --offset2 0x174 */
/* @PostProc: swap_instructions --offset1 0x158 --offset2 0x168 */
/* @PostProc: swap_instructions --offset1 0x15c --offset2 0x16c */
/* @PostProc: swap_instructions --offset1 0x160 --offset2 0x174 */
/* @PostProc: swap_instructions --offset1 0x164 --offset2 0x168 */
/* @PostProc: swap_instructions --offset1 0x168 --offset2 0x16c */
/* @PostProc: swap_instructions --offset1 0x16c --offset2 0x174 */
/* @PostProc: swap_instructions --offset1 0x230 --offset2 0x268 */
/* @PostProc: swap_instructions --offset1 0x234 --offset2 0x268 */
/* @PostProc: swap_instructions --offset1 0x238 --offset2 0x268 */
/* @PostProc: swap_instructions --offset1 0x23c --offset2 0x26c */
/* @PostProc: swap_instructions --offset1 0x240 --offset2 0x268 */
/* @PostProc: swap_instructions --offset1 0x244 --offset2 0x26c */
/* @PostProc: swap_instructions --offset1 0x248 --offset2 0x268 */
/* @PostProc: swap_instructions --offset1 0x24c --offset2 0x26c */
/* @PostProc: swap_instructions --offset1 0x250 --offset2 0x268 */
/* @PostProc: swap_instructions --offset1 0x254 --offset2 0x26c */
/* @PostProc: swap_instructions --offset1 0x258 --offset2 0x268 */
/* @PostProc: swap_instructions --offset1 0x25c --offset2 0x26c */
/* @PostProc: swap_instructions --offset1 0x260 --offset2 0x268 */
/* @PostProc: swap_instructions --offset1 0x264 --offset2 0x26c */
/* `@CompileFlags: -Wc,-pic1` reproduces it natively. */
/* 2026-09-24: `@Compiler: ido52-gas` REFUTED for this TU (Body 231-250, */
#include "types.h"

typedef struct { s16 x, y, z, pad; } Vtx8;
typedef union { u32 w; u8 b[4]; } Col4;
typedef union { u32 w; struct { u16 lo, hi; } h; } UV;

typedef struct {
    Vtx8 v;
    u32 f8;
    u32 f12;
    s32 f16;
    s32 f20;
} WorkSlot;

typedef struct {
    u8 pad0[12];
    u16 f12;
    u16 f14;
    Col4 col;
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
    u8 pad1[2];
    u16 idx0;
    u16 idx1;
    u16 idx2;
} PolyRec;

typedef struct {
    s32 f0;
    s32 *otBase;
    s32 otOfs;
} OtCtx;

extern s32 RotAverageNclip3(Vtx8 *, Vtx8 *, Vtx8 *, s32 *, s32 *, s32 *,
                         s32 *, s32 *, s32 *);
extern s32 RotAverage3(Vtx8 *, Vtx8 *, Vtx8 *, s32 *, s32 *, s32 *,
                         s32 *, s32 *);
extern void ReadSZfifo3(s32 *, s32 *, s32 *);
extern void *RCpolyFT3A(void *, Work *, s32, void *);
extern s32 GsTON;

void *GsTMDdivTF3NL(PolyRec *poly, Vtx8 *vtx, s32 unused, void *out,
                    u32 count, s32 shift, OtCtx *ctx, Work *work)
{
    WorkSlot *p0;
    WorkSlot *p1;
    WorkSlot *p2;
    volatile u32 i;
    s32 otz;
    s32 p;
    s32 flag;
    s32 pad[6];

    p0 = &work->slot[0];
    p1 = &work->slot[1];
    p2 = &work->slot[2];
    work->ptr[0] = p0;
    work->ptr[1] = p1;
    work->ptr[2] = p2;

    i = 0;
    if (count != 0) {
        loop: {
            p0->v = vtx[poly->idx0];
            p1->v = vtx[poly->idx1];
            p2->v = vtx[poly->idx2];
            if ((poly->flag & 2) == 0) {
                if (RotAverageNclip3(&p0->v, &p1->v, &p2->v,
                                  &p0->f16, &p1->f16, &p2->f16,
                                  &p, &otz, &flag) > 0) {
                    ReadSZfifo3(&p0->f20, &p1->f20, &p2->f20);
                    work->ot = ctx->otBase +
                               ((u32)((u16)otz - ctx->otOfs) >> shift);
                    work->col.w = 0x24808080;
                    work->col.b[3] = (u8)((GsTON << 1) | poly->code);
                    work->f12 = poly->uv0.h.hi;
                    work->f14 = poly->uv1.h.hi;
                    p0->f8 = poly->uv0.w;
                    p1->f8 = poly->uv1.w;
                    p2->f8 = poly->col2;
                    out = RCpolyFT3A(out, work, 0, work->tail);
                }
            } else {
                *(s16 *)&otz = RotAverage3(&p0->v, &p1->v, &p2->v,
                                    &p0->f16, &p1->f16, &p2->f16,
                                    &p, &flag);
                ReadSZfifo3(&p0->f20, &p1->f20, &p2->f20);
                work->ot = ctx->otBase +
                           ((u32)((u16)otz - ctx->otOfs) >> shift);
                work->col.w = 0x24808080;
                work->col.b[3] = (u8)((GsTON << 1) | poly->code);
                work->f12 = poly->uv0.h.hi;
                work->f14 = poly->uv1.h.hi;
                p0->f8 = poly->uv0.w;
                p1->f8 = poly->uv1.w;
                p2->f8 = poly->col2;
                out = RCpolyFT3A(out, work, 0, work->tail);
            }
            poly++;
            i += 1;
            if (i >= count) goto done;
            goto loop;
        }
        done:;
    }
    return out;
}
