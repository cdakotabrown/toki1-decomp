/* Function: GsTMDdivNF4 */
/* Address: 0x800AA914 */
/* Size: 608 bytes (152 instructions) */
/* Region: Library (0x80086000+), Sony libgs polygon-list batch emitter */
/* @CompileFlags: -Wc,-pic1 -Wo,-do_opt_saved_regs -Wo,-noheurAB */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: remap_stack_slots --map '52:68,56:72,60:76,64:80,68:84,72:88,76:92,80:96,84:100,148:176,152:180,156:184' */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0x10 */
/* @PostProc: swap_instructions --offset1 0x10 --offset2 0x18 */
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
/* @CompileEnv: IDO52_AS1_NO_FALLTHRU_FILL=all IDO52_AS1_NO_LA_PULL=all IDO52_FORCE_WEB_COLOR=31:s1,2:s2,29:s3,4:s4,8:s5,12:s6,16:s7,22:s8 IDO52_UGEN_TEMP_REG=7:t2,5:t6,14:t7,15:t8,16:t2,27:t8,19:t9,17:t5,29:t2,32:t5,33:t6 */
#include "types.h"

typedef struct {
    u8 d[8];
} GsVERT8;

typedef struct {
    GsVERT8 v;
    u8      pad[8];
    s32     a;
    s32     b;
} GsSLOT;

typedef union {
    u32 word;
    struct { u8 r, g, b, code; } c;
} GsCW;

typedef struct {
    s32     pad0[4];
    GsCW    cw;
    u32    *tag;
    GsSLOT  slot[4];
    s32     work[30];
    GsSLOT *pslot[4];
} GsWORK;

typedef struct {
    u8  pad0[3];
    u8  code;
    u32 rgb;
    u16 v0;
    u16 v1;
    u16 v2;
    u16 v3;
} GsPOLY;

typedef struct {
    u32  length;
    u32 *org;
    u32  offset;
    u32  point;
} GsOT;

extern u32 GsTON;

extern s32 RotAverageNclip4(GsSLOT *, GsSLOT *, GsSLOT *, GsSLOT *,
                         s32 *, s32 *, s32 *, s32 *, s32 *, s32 *, s32 *);
extern void ReadSZfifo4(s32 *, s32 *, s32 *, s32 *);
extern void *RCpolyF4A(void *, GsWORK *, s32, s32 *);

void *GsTMDdivNF4(GsPOLY *hdr, GsVERT8 *base, void *out, u32 count,
                    s32 shift, GsOT *ot, GsWORK *w)
{
    GsSLOT *p0;
    GsSLOT *p1;
    GsSLOT *p2;
    GsSLOT *p3;
    u32 i;
    s32 unused[3];
    s32 otz;
    s32 p;
    s32 flag;

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
        do {
            p0->v = base[hdr->v0];
            p1->v = base[hdr->v1];
            p2->v = base[hdr->v2];
            p3->v = base[hdr->v3];
            if (RotAverageNclip4(p0, p1, p2, p3, &p0->a, &p1->a, &p2->a, &p3->a,
                              &p, &otz, &flag) > 0) {
                ReadSZfifo4(&p0->b, &p1->b, &p2->b, &p3->b);
                w->tag = ot->org + ((u32)((u16)otz - ot->offset) >> shift);
                w->cw.word = hdr->rgb;
                w->cw.c.code = (u8)((GsTON << 1) | hdr->code);
                out = RCpolyF4A(out, w, 0, w->work);
            }
            hdr++;
            i++;
        } while (i < count);
    }
    return out;
}
