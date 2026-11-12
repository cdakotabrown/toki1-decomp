/* Address: 0x800AAD20, Size: 620 bytes (155 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @CompileEnv: IDO52_AS1_NO_FALLTHRU_FILL=all IDO52_AS1_NO_LA_PULL=all IDO52_FORCE_WEB_COLOR=2:s2,4:s3,8:s4,12:s5,17:s6,24:s7,42:s8 IDO52_FORCE_WEB_NOSPILL=109,110 IDO52_UGEN_TEMP_REG=9:t4,7:t8,16:t9,17:t2,18:t4,21:t3,19:t7,25:t7,28:t3 */
/* @CompileFlags: -Wc,-pic1 -Wo,-do_opt_saved_regs */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: remap_stack_slots --map '68:92,72:96,76:100,80:104,84:108,88:112,92:116,96:120,100:124,124:132,128:136,132:140,136:144,140:148,144:152,148:156' */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0x10 */
/* @PostProc: swap_instructions --offset1 0x10 --offset2 0x14 */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x1c */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x1c */
/* @PostProc: swap_instructions --offset1 0x20 --offset2 0x24 */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x78 */
/* @PostProc: swap_instructions --offset1 0x64 --offset2 0x78 */
/* @PostProc: swap_instructions --offset1 0x68 --offset2 0x90 */
/* @PostProc: swap_instructions --offset1 0x6c --offset2 0x78 */
/* @PostProc: swap_instructions --offset1 0x70 --offset2 0x8c */
/* @PostProc: swap_instructions --offset1 0x74 --offset2 0x90 */
/* @PostProc: swap_instructions --offset1 0x78 --offset2 0x88 */
/* @PostProc: swap_instructions --offset1 0x7c --offset2 0x88 */
/* @PostProc: swap_instructions --offset1 0x80 --offset2 0x84 */
/* @PostProc: swap_instructions --offset1 0x84 --offset2 0x8c */
/* @PostProc: swap_instructions --offset1 0x88 --offset2 0x90 */
/* @PostProc: swap_instructions --offset1 0x8c --offset2 0x90 */
/* @PostProc: patch_insn_reg_field 0x150:rs:t7:fp 0x150:rt:fp:t7 0x160:rs:t6:fp 0x160:rt:fp:t6 0x170:rs:t3:fp 0x170:rt:fp:t3 */
/* @PostProc: swap_instructions --offset1 0x15c --offset2 0x190 */
/* @PostProc: swap_instructions --offset1 0x160 --offset2 0x194 */
/* @PostProc: swap_instructions --offset1 0x164 --offset2 0x198 */
/* @PostProc: swap_instructions --offset1 0x168 --offset2 0x19c */
/* @PostProc: swap_instructions --offset1 0x16c --offset2 0x190 */
/* @PostProc: swap_instructions --offset1 0x170 --offset2 0x194 */
/* @PostProc: swap_instructions --offset1 0x174 --offset2 0x198 */
/* @PostProc: swap_instructions --offset1 0x178 --offset2 0x19c */
/* @PostProc: swap_instructions --offset1 0x17c --offset2 0x1a0 */
/* @PostProc: swap_instructions --offset1 0x180 --offset2 0x1a8 */
/* @PostProc: swap_instructions --offset1 0x184 --offset2 0x190 */
/* @PostProc: swap_instructions --offset1 0x188 --offset2 0x194 */
/* @PostProc: swap_instructions --offset1 0x190 --offset2 0x198 */
/* @PostProc: swap_instructions --offset1 0x194 --offset2 0x19c */
/* @PostProc: swap_instructions --offset1 0x198 --offset2 0x1a0 */
/* @PostProc: swap_instructions --offset1 0x19c --offset2 0x1a8 */
#include "types.h"

typedef struct { s16 x, y, z, pad; } Vtx8;

typedef struct {
    Vtx8 v;
    s32 f8;
    s32 col;
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
    u16 n;
    u16 i;
} PolyVtx;

typedef struct {
    u8 pad0[3];
    u8 code;
    s32 col;
    PolyVtx vt[3];
} PolyG3N;

typedef struct {
    s32 f0;
    s32 *otBase;
    s32 otOfs;
} OtCtx;

extern s32 RotAverageNclipColorCol3(Vtx8 *, Vtx8 *, Vtx8 *, Vtx8 *, Vtx8 *, Vtx8 *,
                         s32 *, s32 *, s32 *, s32 *, s32 *, s32 *, s32 *,
                         s32 *, s32 *);
extern void ReadSZfifo3(s32 *, s32 *, s32 *);
extern void *RCpolyG3A(void *, Work *, s32, void *);
extern s32 GsTON;

void *GsTMDdivG3L(PolyG3N *poly, Vtx8 *vtx, Vtx8 *nrm, void *out,
                    u32 count, s32 shift, OtCtx *ctx, Work *work)
{
    s32 unused;
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
        p0->v = vtx[poly->vt[0].i];
        p1->v = vtx[poly->vt[1].i];
        p2->v = vtx[poly->vt[2].i];
        if (RotAverageNclipColorCol3(&p0->v, &p1->v, &p2->v, &nrm[poly->vt[0].n],
                          &nrm[poly->vt[1].n], &nrm[poly->vt[2].n],
                          &poly->col, &p0->f16, &p1->f16, &p2->f16,
                          &p0->col, &p1->col, &p2->col, &otz, &flag) > 0) {
            ReadSZfifo3(&p0->f20, &p1->f20, &p2->f20);
            work->ot = ctx->otBase +
                       ((u32)((u16)otz - ctx->otOfs) >> shift);
            work->code = (u8)((GsTON << 1) | poly->code);
            out = RCpolyG3A(out, work, 0, work->tail);
        }
        poly++;
        i++;
    } while (i < count);
    }
    return out;
}
