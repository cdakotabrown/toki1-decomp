/* Address: 0x800AAB74 */
/* Size: 428 bytes (105 instructions + 2 alignment words) */
/* Region: library (IDO 5.2, native as1, pic0 -- the region default) */
/* @CompileFlags: -Wc,-pic0 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR_NOSPLIT=22,25 IDO52_FORCE_WEB_COLOR=2:v1,4:t0,8:t1,12:t2,16:t3,22:t4,25:t5 IDO52_AS1_NO_PASS1_PROP=all IDO52_AS1_NO_COPY_PROP=all IDO52_AS1_KEEP_DEAD_RELOAD=all IDO52_AS1_NO_FALLTHRU_FILL=all IDO52_AS1_XBB_SKIP_CAND=all IDO52_UGEN_TEMP_REG=2:t6,4:t7,5:t8,7:t9,8:t6,10:t7,11:t9,12:t8,13:t7,14:t6,15:t9,16:t8,17:t7,18:t6,19:t9 IDO52_AS1_TIE_TAKE=134,150,157,163,168,178 IDO52_AS1_TIE_REJECT=141,167 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: remap_stack_slots --map 52:76,60:84,64:88,68:92,72:96,104:128,108:132,112:136,128:152,132:156,136:160,140:164,144:168,148:172 */
/* @PostProc: patch_stack_complete --old 120 --new 144 */
#include "types.h"

typedef struct {
    s16 vx, vy, vz, pad;
} SVEC;

typedef struct {
    u8 r, g, b, cd;
} DIVRGB;

typedef struct {
    SVEC xyz;
    s32 unk08;
    s32 unk0C;
    s32 sxy;
    s32 sz;
} DIVVTX;

typedef struct {
    s32 unk00[4];
    DIVRGB rgb;
    s32 attr;
    DIVVTX v[9];
    DIVVTX *p[4];
} DIVWORK4;

extern s32 RotAverageNclip4(DIVVTX *v0, DIVVTX *v1, DIVVTX *v2, DIVVTX *v3,
                         s32 *sxy0, s32 *sxy1, s32 *sxy2, s32 *sxy3,
                         s32 *p, s32 *otz, s32 *flag);
extern void ReadSZfifo4(s32 *sz0, s32 *sz1, s32 *sz2, s32 *sz3);
extern void *RCpolyF4A(void *out, DIVWORK4 *w, s32 mode, DIVVTX *sub);

void *DivideF4(SVEC *a, SVEC *b, SVEC *c, SVEC *d,
                    DIVRGB *col, void *out, s32 attr, DIVWORK4 *w)
{
    s32 nclip;
    s32 otz;
    s32 p;
    s32 flag;
    s32 u1;
    s32 u2;
    s32 u3;
    DIVVTX *p0;
    DIVVTX *p1;
    DIVVTX *p2;
    DIVVTX *p3;

    p0 = &w->v[0];
    p1 = &w->v[1];
    p2 = &w->v[2];
    p3 = &w->v[3];
    w->p[0] = p0;
    w->p[1] = p1;
    w->p[2] = p2;
    w->p[3] = p3;
    p0->xyz = *a;
    p1->xyz = *b;
    p2->xyz = *c;
    p3->xyz = *d;
    nclip = RotAverageNclip4(p0, p1, p2, p3,
                          &w->v[0].sxy, &w->v[1].sxy, &w->v[2].sxy,
                          &w->v[3].sxy, &p, &otz, &flag);
    if (nclip > 0) {
        ReadSZfifo4(&p0->sz, &p1->sz, &p2->sz, &p3->sz);
        w->attr = attr;
        w->rgb = *col;
        out = RCpolyF4A(out, w, 0, &w->v[4]);
    }
    return out;
}
