/* Address: 0x800AD2AC, Size: 488 bytes (122 instructions; the 3 zero words to */
/* Region: Library (0x80086000+), LIBGS-style sort family */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: remap_stack_slots --map '44:68,68:76,72:80,76:84' */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=2:v1,4:t0,8:t1,12:t2,17:t3,20:t4,23:t5,49:t3,91:t4 IDO52_FORCE_WEB_COLOR_NOSPLIT=17,20,23 IDO52_AS1_NO_COPY_PROP=all IDO52_AS1_NO_PASS1_PROP=all IDO52_AS1_NO_LA_PULL=all IDO52_AS1_KEEP_DEAD_RELOAD=0 IDO52_UGEN_TEMP_REG=1:t7,6:t8,7:t9,4:t7,13:t6 IDO52_AS1_TIE_REJECT=47 IDO52_AS1_TIE_TAKE=213,222,230,237,276,281 */
#include "types.h"

typedef struct { s16 x, y, z, pad; } Vtx8;
typedef struct { u8 r, g, b, code; } Col4;

typedef struct {
    Vtx8 v;
    s32 f8;
    s32 f12;
    s32 f16;
    s32 f20;
} WorkSlot;

typedef struct {
    u8 pad0[12];
    s16 f12;
    s16 f14;
    Col4 col;
    s32 f20;
    WorkSlot slot[3];
    u8 tail[72];
    WorkSlot *ptr[3];
} Work;

extern s32 RotAverageNclip3(Vtx8 *, Vtx8 *, Vtx8 *, s32 *, s32 *, s32 *, s32 *, s32 *, s32 *);
extern void ReadSZfifo3(s32 *, s32 *, s32 *);
extern void *RCpolyGT3A(void *, Work *, s32, void *);

void *DivideGT3(Vtx8 *v0, Vtx8 *v1, Vtx8 *v2, u32 *n0, u32 *n1, u32 *n2, Col4 *col, s32 *m1, s32 *m2, void *out, s32 arg_tag, Work *work)
{
    s32 w_;
    s32 z;
    s32 y;
    s32 x;
    s32 pad[5];
    WorkSlot *p0;
    WorkSlot *p1;
    WorkSlot *p2;

    p0 = &work->slot[0];
    p1 = &work->slot[1];
    p2 = &work->slot[2];
    work->ptr[0] = p0;
    work->ptr[1] = p1;
    work->ptr[2] = p2;
    p0->v = *v0;
    p1->v = *v1;
    p2->v = *v2;

    if (RotAverageNclip3(&p0->v, &p1->v, &p2->v, &p0->f16, &p1->f16, &p2->f16, &y, &z, &x) > 0) {
        ReadSZfifo3(&p0->f20, &p1->f20, &p2->f20);
        work->f20 = arg_tag;
        work->col = *col;
        work->f12 = (s16)(*n0 >> 16);
        work->f14 = (s16)(*n1 >> 16);
        p0->f8 = *n0;
        p1->f8 = *n1;
        p2->f8 = *n2;
        p0->f12 = *(s32 *)col;
        p1->f12 = *m1;
        p2->f12 = *m2;
        out = RCpolyGT3A(out, work, 0, &work->tail[0]);
    }
    return out;
}
