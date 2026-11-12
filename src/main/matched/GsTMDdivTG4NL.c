/* Function: GsTMDdivTG4NL */
/* Address: 0x800AF538 */
/* Size: 708 bytes (177 instructions) */
/* Region: Library (0x80086000+), Sony libgs polygon-list batch emitter */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: remap_stack_slots --map '56:72,60:76,64:80,68:84,72:88,76:92,80:96,84:100,152:184,156:188,160:192,172:196' */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0x10 */
/* @PostProc: swap_instructions --offset1 0x10 --offset2 0x18 */
/* @PostProc: swap_instructions --offset1 0x1c --offset2 0x20 */
/* @PostProc: swap_instructions --offset1 0x68 --offset2 0x7c */
/* @PostProc: swap_instructions --offset1 0x6c --offset2 0x7c */
/* @PostProc: swap_instructions --offset1 0x70 --offset2 0x7c */
/* @PostProc: swap_instructions --offset1 0x74 --offset2 0x8c */
/* @PostProc: swap_instructions --offset1 0x78 --offset2 0x7c */
/* @PostProc: swap_instructions --offset1 0x7c --offset2 0x88 */
/* @PostProc: swap_instructions --offset1 0x80 --offset2 0x8c */
/* @PostProc: swap_instructions --offset1 0x84 --offset2 0x88 */
/* @PostProc: swap_instructions --offset1 0x88 --offset2 0x8c */
/* @PostProc: swap_instructions --offset1 0x178 --offset2 0x17c */
/* @PostProc: swap_instructions --offset1 0x17c --offset2 0x180 */
/* @PostProc: swap_instructions --offset1 0x180 --offset2 0x184 */
/* @PostProc: swap_instructions --offset1 0x184 --offset2 0x188 */
/* @PostProc: patch_insn_reg_field 0x17c:rt:t4:t3 0x188:rt:t3:t8 0x19c:rt:t3:t8 0x1cc:rt:t5:t4 0x1d0:rt:t6:t5 0x1d8:rs:t5:t4 0x1d8:rt:t6:t5 0x1d8:rd:t8:t6 0x1e0:rt:t8:t6 0x1e0:rd:t3:t8 0x1e4:rt:t3:t8 0x158:rt:t4:t3 */
/* @PostProc: swap_instructions --offset1 0x134 --offset2 0x158 */
/* @CompileFlags: -Wc,-pic1 -Wo,-do_opt_saved_regs -Wo,-varref,29 */
/* @CompileEnv: IDO52_AS1_KEEP_DEAD_RELOAD=0 IDO52_AS1_NO_FALLTHRU_FILL=all IDO52_AS1_NO_LA_PULL=all IDO52_FORCE_WEB_COLOR=2:s2,4:s4,8:s5,12:s6,16:s7,32:s8,142:s3,22:v1 IDO52_FORCE_WEB_NOSPILL=136,137 IDO52_UGEN_TEMP_REG=8:t3,6:t7,15:t8,16:t9,17:t3,28:t9,20:t2,18:t6,38:t3,32:t6,42:t7 */
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
    u32     c;
    s32     a;
    s32     b;
} GsSLOT4;

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
    GsSLOT4  slot[4];
    s32      work[30];
    GsSLOT4 *pslot[4];
} GsWORK4;

typedef struct {
    u8   pad0[3];
    u8   code;
    GsUV uv0;
    GsUV uv1;
    GsUV uv2;
    GsUV uv3;
    u16  n0;
    u16  v0;
    u16  n1;
    u16  v1;
    u16  n2;
    u16  v2;
    u16  n3;
    u16  v3;
} GsPOLY4;

typedef struct {
    u32  length;
    u32 *org;
    u32  offset;
    u32  point;
} GsOT;

extern u32 GsTON;

extern s32 RotAverageNclip4(GsSLOT4 *, GsSLOT4 *, GsSLOT4 *, GsSLOT4 *,
                         s32 *, s32 *, s32 *, s32 *, s32 *, u16 *, s32 *);
extern void ReadSZfifo4(s32 *, s32 *, s32 *, s32 *);
extern void *RCpolyGT4A(void *, GsWORK4 *, s32, s32 *);

void *GsTMDdivTG4NL(GsPOLY4 *hdr, GsVERT8 *base, void *unused, void *out,
                    u32 count, s32 shift, GsOT *ot, GsWORK4 *w)
{
    GsSLOT4 *p0;
    GsSLOT4 *p1;
    GsSLOT4 *p2;
    GsSLOT4 *p3;
    s32 *pk;
    u32 code;
    u32 i;
    s32 pad[2];
    s32 x2;
    s32 x1;
    s32 x0;

    p0 = &w->slot[0];
    p1 = &w->slot[1];
    p2 = &w->slot[2];
    p3 = &w->slot[3];
    w->pslot[0] = p0;
    w->pslot[1] = p1;
    w->pslot[2] = p2;
    w->pslot[3] = p3;
    i = 0;
    if (count != 0) {
        code = 0x3C808080;
        do {
            p0->v = base[hdr->v0];
            p1->v = base[hdr->v1];
            p2->v = base[hdr->v2];
            p3->v = base[hdr->v3];
            if (RotAverageNclip4(p0, p1, p2, p3, &p0->a, &p1->a, &p2->a, &p3->a,
                              &x1, (u16 *)&x2, &x0) > 0) {
                pk = w->work;
                ReadSZfifo4(&p0->b, &p1->b, &p2->b, &p3->b);
                w->tag = ot->org + ((u32)((u16)x2 - ot->offset) >> shift);
                w->cw.c.code = (u8)((GsTON << 1) | hdr->code);
                w->clut = hdr->uv0.t.cb;
                w->tpage = hdr->uv1.t.cb;
                p0->c = code;
                p1->c = code;
                p2->c = code;
                p3->c = code;
                p0->uv = hdr->uv0.word;
                p1->uv = hdr->uv1.word;
                p2->uv = hdr->uv2.word;
                p3->uv = hdr->uv3.word;
                out = RCpolyGT4A(out, w, 0, pk);
            }
            hdr++;
            i++;
        } while (i < count);
    }
    return out;
}
