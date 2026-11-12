/* Address: 0x800ADFA4, Size: 516 bytes (129 instructions; the recorded 524 held 2 alignment nops) */
/* Region: Library (0x80086000+), LIBGS-style sort family */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=2:v1,4:t0,8:t1,12:t2,16:t3,22:t4,25:t5 IDO52_FORCE_WEB_COLOR_NOSPLIT=22,25 IDO52_AS1_NO_COPY_PROP=all IDO52_AS1_NO_PASS1_PROP=all IDO52_AS1_NO_LA_PULL=all IDO52_AS1_KEEP_DEAD_RELOAD=0 IDO52_AS1_TIE_TAKE=134,142,150,157,163,168,178,202,209,215 IDO52_UGEN_TEMP_REG=2:t6,4:t7,7:t9,10:t7,11:t9,13:t7,21:t8 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: remap_stack_slots --map '52:84,76:92,80:96,84:100,88:104' */
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
    WorkSlot slot[4];
    u8 tail[120];
    WorkSlot *ptr[4];
} Work;

extern s32 RotAverageNclip4(Vtx8 *, Vtx8 *, Vtx8 *, Vtx8 *, s32 *, s32 *, s32 *, s32 *, s32 *, s32 *, s32 *);
extern void ReadSZfifo4(s32 *, s32 *, s32 *, s32 *);
extern void *RCpolyG4A(void *, Work *, s32, void *);

void *DivideG4(Vtx8 *v0, Vtx8 *v1, Vtx8 *v2, Vtx8 *v3, Col4 *col, s32 *m1, s32 *m2, s32 *m3, void *out, s32 arg_tag, Work *work)
{
    s32 w_;
    s32 z;
    s32 y;
    s32 x;
    s32 pad[7];
    WorkSlot *p0;
    WorkSlot *p1;
    WorkSlot *p2;
    WorkSlot *p3;

    p0 = &work->slot[0];
    p1 = &work->slot[1];
    p2 = &work->slot[2];
    p3 = &work->slot[3];
    work->ptr[0] = p0;
    work->ptr[1] = p1;
    work->ptr[2] = p2;
    work->ptr[3] = p3;
    p0->v = *v0;
    p1->v = *v1;
    p2->v = *v2;
    p3->v = *v3;

    if (RotAverageNclip4(&p0->v, &p1->v, &p2->v, &p3->v, &p0->f16, &p1->f16, &p2->f16, &p3->f16, &y, &z, &x) > 0) {
        ReadSZfifo4(&p0->f20, &p1->f20, &p2->f20, &p3->f20);
        work->f20 = arg_tag;
        work->col = *col;
        p0->f12 = *(s32 *)col;
        p1->f12 = *m1;
        p2->f12 = *m2;
        p3->f12 = *m3;
        out = RCpolyG4A(out, work, 0, &work->tail[0]);
    }
    return out;
}
