/* Function: GsTMDdivTG3LFG */
/* Address: 0x800ACB5C */
/* Size: 684 bytes (171 instructions) */
/* Region: Library (0x80086000+), Sony libgs polygon-list batch emitter */
/* @CompileFlags: -Wc,-pic1 -Wo,-do_opt_saved_regs */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: remap_stack_slots --map '68:92,72:96,76:100,80:104,84:108,88:112,92:116,96:120,100:124,120:128,144:152,140:148,136:144,132:140,128:136,124:132,200:196,204:200,212:208' */
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
/* @CompileEnv: IDO52_AS1_NO_FALLTHRU_FILL=all IDO52_AS1_NO_LA_PULL=all IDO52_FORCE_WEB_COLOR=5:s2,7:s3,11:s4,15:s5,20:s6,27:s7,43:s8,128:t1 IDO52_FORCE_WEB_NOSPILL=125,126,127 IDO52_UGEN_TEMP_REG=10:t5,8:t9,17:t2,18:t3,19:t5,22:t4,20:t8,26:t8,29:t4 */
#include "types.h"

typedef struct {
    u8 d[8];
} GsVERT8;

typedef union {
    u32 word;
    struct { u8 u, v; u16 cb; } t;
} GsUV;

typedef struct {
    GsVERT8 v;
    u32     uv;
    s32     c;
    s32     a;
    s32     b;
} GsSLOT3;

typedef union {
    u32 word;
    struct { u8 r, g, b, code; } c;
} GsCW;

typedef struct {
    s32      pad0[3];
    u16      clut;
    u16      tpage;
    GsCW     cw;
    u32     *tag;
    GsSLOT3  slot[3];
    s32      work[18];
    GsSLOT3 *pslot[3];
} GsWORK3;

typedef struct {
    u8   pad0[3];
    u8   code;
    GsUV uv0;
    GsUV uv1;
    GsUV uv2;
    u16  n0;
    u16  v0;
    u16  n1;
    u16  v1;
    u16  n2;
    u16  v2;
} GsPOLY3;

typedef struct {
    u32  length;
    u32 *org;
    u32  offset;
    u32  point;
} GsOT;

extern u32 GsTON;

extern s32 RotAverageNclipColorDpq3(GsSLOT3 *, GsSLOT3 *, GsSLOT3 *, s16 *,
                         s16 *, s16 *, u32 *,
                         s32 *, s32 *, s32 *, s32 *, s32 *, s32 *,
                         s32 *, s32 *);
extern void ReadSZfifo3(s32 *, s32 *, s32 *);
extern void *RCpolyGT3A(void *, GsWORK3 *, s32, s32 *);

void *GsTMDdivTG3LFG(GsPOLY3 *hdr, GsVERT8 *base, s16 (*uvb)[4], void *out,
                    u32 count, s32 shift, GsOT *ot, GsWORK3 *w)
{
    s32 otz;
    s32 unused;
    s32 y;
    u32 code;
    s32 pad0;
    s32 pad1;
    s32 pad2;
    s32 pad3;
    GsSLOT3 *p0;
    GsSLOT3 *p1;
    GsSLOT3 *p2;
    s32 *pk;
    u32 i;

    code = 0x34808080;
    p0 = &w->slot[0];
    p1 = &w->slot[1];
    p2 = &w->slot[2];
    w->pslot[0] = p0;
    w->pslot[1] = p1;
    w->pslot[2] = p2;
    i = 0;
    if (count != 0) {
        do {
            p0->v = base[hdr->v0];
            p1->v = base[hdr->v1];
            p2->v = base[hdr->v2];
            if (RotAverageNclipColorDpq3(p0, p1, p2, uvb[hdr->n0],
                              uvb[hdr->n1], uvb[hdr->n2], &code,
                              &p0->a, &p1->a, &p2->a,
                              &p0->c, &p1->c, &p2->c,
                              &otz, &y) > 0) {
                pk = w->work;
                ReadSZfifo3(&p0->b, &p1->b, &p2->b);
                w->tag = ot->org + (((u16)otz - ot->offset) >> shift);
                w->cw.c.code = (u8)((GsTON << 1) | hdr->code);
                w->clut = hdr->uv0.t.cb;
                w->tpage = hdr->uv1.t.cb;
                p0->uv = hdr->uv0.word;
                p1->uv = hdr->uv1.word;
                p2->uv = hdr->uv2.word;
                out = RCpolyGT3A(out, w, 0, pk);
            }
            hdr++;
            i++;
        } while (i < count);
    }
    return out;
}
