/* Address: 0x800A9A00, Size: 592 bytes (148 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @CompileEnv: IDO52_AS1_NO_FALLTHRU_FILL=all IDO52_AS1_NO_LA_PULL=all IDO52_FORCE_WEB_COLOR=26:s1,2:s2,4:s5,8:s3,12:s4,17:s6,24:s7 IDO52_FORCE_WEB_NOSPILL=101,102,105 IDO52_UGEN_TEMP_REG=9:t4,7:t8,23:t6,17:t9,28:t2,35:t2 */
/* @CompileFlags: -Wc,-pic1 -Wo,-do_opt_saved_regs */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: remap_stack_slots --map '72:96,76:100,80:104,84:108,88:112,92:116,96:120,100:124,120:128,124:132,128:136,132:140,136:144,140:148,144:152,152:156' */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0xc */
/* @PostProc: swap_instructions --offset1 0xc --offset2 0x10 */
/* @PostProc: swap_instructions --offset1 0x10 --offset2 0x14 */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x18 */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x7c */
/* @PostProc: swap_instructions --offset1 0x64 --offset2 0x7c */
/* @PostProc: swap_instructions --offset1 0x68 --offset2 0x94 */
/* @PostProc: swap_instructions --offset1 0x6c --offset2 0x7c */
/* @PostProc: swap_instructions --offset1 0x70 --offset2 0x90 */
/* @PostProc: swap_instructions --offset1 0x74 --offset2 0x94 */
/* @PostProc: swap_instructions --offset1 0x78 --offset2 0x8c */
/* @PostProc: swap_instructions --offset1 0x80 --offset2 0x88 */
/* @PostProc: swap_instructions --offset1 0x84 --offset2 0x90 */
/* @PostProc: swap_instructions --offset1 0x88 --offset2 0x94 */
/* @PostProc: swap_instructions --offset1 0x90 --offset2 0x94 */
/* @PostProc: swap_instructions --offset1 0x14c --offset2 0x170 */
/* @PostProc: swap_instructions --offset1 0x150 --offset2 0x174 */
/* @PostProc: swap_instructions --offset1 0x154 --offset2 0x178 */
/* @PostProc: swap_instructions --offset1 0x158 --offset2 0x17c */
/* @PostProc: swap_instructions --offset1 0x15c --offset2 0x180 */
/* @PostProc: swap_instructions --offset1 0x160 --offset2 0x184 */
/* @PostProc: swap_instructions --offset1 0x164 --offset2 0x170 */
/* @PostProc: swap_instructions --offset1 0x168 --offset2 0x174 */
/* @PostProc: swap_instructions --offset1 0x16c --offset2 0x178 */
/* @PostProc: swap_instructions --offset1 0x170 --offset2 0x188 */
/* @PostProc: swap_instructions --offset1 0x174 --offset2 0x17c */
/* @PostProc: swap_instructions --offset1 0x178 --offset2 0x190 */
/* @PostProc: swap_instructions --offset1 0x17c --offset2 0x180 */
/* @PostProc: swap_instructions --offset1 0x180 --offset2 0x184 */
/* @PostProc: swap_instructions --offset1 0x184 --offset2 0x188 */
/* @PostProc: patch_insn_reg_field 0x110:rt:t3:t9 0x118:rt:t3:t9 0x118:rd:t9:t2 0x11c:rt:t9:t2 0x134:rt:t2:t3 0x138:rt:t2:t3 0x140:rt:t2:t3 0x144:rt:t2:t3 0x150:rt:t3:t7 0x164:rt:t7:t5 0x168:rt:t5:t2 0x170:rt:t5:t2 0x17c:rt:t2:t3 0x180:rs:t7:t5 0x190:rt:t2:t3 0x1b4:rt:t7:t5 0x1b8:rt:t3:t7 0x1bc:rt:t7:t5 0x1c0:rt:t5:t3 0x1c4:rs:t3:t7 0x1cc:rs:t5:t3 0x1e0:rd:t7:t5 0x1e4:rs:t7:t5 0x1e4:rd:t3:t7 0x1e8:rt:t3:t7 0x128:rt:t3:t7 */
/* @PostProc: swap_instructions --offset1 0x104 --offset2 0x128 */
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
    u8 pad0[16];
    s32 col;
    s32 *ot;
    WorkSlot slot[3];
    u8 tail[72];
    WorkSlot *ptr[3];
} Work;

typedef struct {
    u8 pad0[3];
    u8 code;
    s32 col;
    u16 n0;
    u16 i0;
    u16 i1;
    u16 i2;
} PolyF3N;

typedef struct {
    s32 f0;
    s32 *otBase;
    s32 otOfs;
} OtCtx;

extern s32 RotAverageNclipColorDpq3(Vtx8 *, Vtx8 *, Vtx8 *, Vtx8 *, Vtx8 *, Vtx8 *,
                         s32 *, s32 *, s32 *, s32 *, s32 *, s32 *, s32 *,
                         s32 *, s32 *);
extern void ReadSZfifo3(s32 *, s32 *, s32 *);
extern void *func_800B12E8(void *, Work *, s32, void *);
extern s32 GsTON;

void *GsTMDdivF3LFG(PolyF3N *poly, Vtx8 *vtx, Vtx8 *nrm, void *out,
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
        p0->v = vtx[poly->i0];
        p1->v = vtx[poly->i1];
        p2->v = vtx[poly->i2];
        if (RotAverageNclipColorDpq3(&p0->v, &p1->v, &p2->v, &((Vtx8 *)nrm)[poly->n0],
                          &((Vtx8 *)nrm)[poly->n0], &((Vtx8 *)nrm)[poly->n0], &poly->col,
                          &p0->f16, &p1->f16, &p2->f16, &work->col,
                          &p1->col, &p2->col, &otz, &flag) > 0) {
            ReadSZfifo3(&p0->f20, &p1->f20, &p2->f20);
            work->ot = ctx->otBase +
                       ((u32)((u16)otz - ctx->otOfs) >> shift);
            ((u8 *)&work->col)[3] = (u8)((GsTON << 1) | poly->code);
            out = func_800B12E8(out, work, 0, work->tail);
        }
        poly++;
        i++;
    } while (i < count);
    }
    return out;
}
