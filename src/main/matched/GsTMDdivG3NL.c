/* Address: 0x800AB1F8, Size: 540 bytes (135 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @CompileFlags: -Wc,-pic1 -Wo,-do_opt_saved_regs */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: remap_stack_slots --map '48:56,52:60,56:64,60:68,64:72,68:76,72:80,76:84,100:92,104:96,108:100,112:104,116:108' */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0xc */
/* @PostProc: swap_instructions --offset1 0xc --offset2 0x10 */
/* @PostProc: swap_instructions --offset1 0x10 --offset2 0x14 */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x18 */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x70 */
/* @PostProc: swap_instructions --offset1 0x64 --offset2 0x70 */
/* @PostProc: swap_instructions --offset1 0x68 --offset2 0x7c */
/* @PostProc: swap_instructions --offset1 0x6c --offset2 0x70 */
/* @PostProc: swap_instructions --offset1 0x70 --offset2 0x7c */
/* @PostProc: swap_instructions --offset1 0x74 --offset2 0x7c */
/* @PostProc: swap_instructions --offset1 0x78 --offset2 0x7c */
/* @PostProc: patch_insn_reg_field 0x118:rt:t2:t8 0x11c:rt:t2:t8 0x124:rt:t2:t8 0x128:rt:t2:t8 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=2:s2,4:s3,8:s4,12:s5,17:s6,24:s7 IDO52_AS1_NO_FALLTHRU_FILL=all IDO52_AS1_NO_LA_PULL=all IDO52_UGEN_TEMP_REG=7:t0,11:t9,5:t7,12:t0,22:t1,16:t7,23:t2,24:t4 */
#include "types.h"

typedef struct { s16 x, y, z, pad; } Vtx8;
typedef struct {
    Vtx8 v;
    s32 f8;
    u32 col;
    s32 f16;
    s32 f20;
} WorkSlot;

typedef struct {
    u8 pad0[19];
    u8 code;
    s32 *ot;
    WorkSlot slot[3];
    u8 tail[72];
    WorkSlot *ptr[3];
} Work;

typedef struct {
    u16 uv;
    u16 idx;
} PolyVtx;

typedef struct {
    u8 pad0[3];
    u8 code;
    u32 col;
    PolyVtx vt[3];
} PolyGT3;

typedef struct {
    s32 f0;
    s32 *otBase;
    s32 otOfs;
} OtCtx;

extern s32 RotAverageNclip3(Vtx8 *, Vtx8 *, Vtx8 *, s32 *, s32 *, s32 *,
                         s32 *, s32 *, s32 *);
extern void ReadSZfifo3(s32 *, s32 *, s32 *);
extern void *RCpolyG3A(void *, Work *, s32, void *);
extern s32 GsTON;

void *GsTMDdivG3NL(PolyGT3 *poly, Vtx8 *vtx, void *unusedArg, void *out,
                    u32 count, s32 shift, OtCtx *ctx, Work *work)
{
    s32 unused;
    s32 otz;
    s32 p;
    s32 flag;
    WorkSlot *p0;
    WorkSlot *p1;
    WorkSlot *p2;
    u32 i;

    p0 = &work->slot[0];
    p1 = &work->slot[1];
    p2 = &work->slot[2];
    work->ptr[0] = p0;
    work->ptr[1] = p1;
    work->ptr[2] = p2;

    i = 0;
    if (count != 0) {
    do {
        p0->v = vtx[poly->vt[0].idx];
        p1->v = vtx[poly->vt[1].idx];
        p2->v = vtx[poly->vt[2].idx];
        if (RotAverageNclip3(&p0->v, &p1->v, &p2->v, &p0->f16, &p1->f16,
                          &p2->f16, &p, &otz, &flag) > 0) {
            ReadSZfifo3(&p0->f20, &p1->f20, &p2->f20);
            work->ot = ctx->otBase +
                       ((u32)((u16)otz - ctx->otOfs) >> shift);
            work->code = (u8)((GsTON << 1) | poly->code);
            p0->col = poly->col;
            p1->col = poly->col;
            p2->col = poly->col;
            out = RCpolyG3A(out, work, 0, work->tail);
        }
        poly++;
        i++;
    } while (i < count);
    }
    return out;
}
