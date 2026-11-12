/* Address: 0x800AB630 */
/* Size: 404 bytes (101 instructions) */
/* Region: Library (0x80086000-0x800BFFFF) */
/* Compiler: IDO 5.2 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: remap_stack_slots --map '44:60' */
/* @CompileEnv: IDO52_AS1_NO_LA_PULL=all IDO52_AS1_NO_COPY_PROP=all IDO52_FORCE_WEB_COLOR=2:v1,4:t0,8:t1,12:t2,17:t3,20:t4,23:t5 IDO52_FORCE_WEB_COLOR_NOSPLIT=17,20,23 IDO52_AS1_NO_PASS1_PROP=all IDO52_AS1_KEEP_DEAD_RELOAD=0 IDO52_UGEN_TEMP_REG=1:t7,6:t8,7:t9,4:t7,13:t6 IDO52_AS1_TIE_REJECT=47 IDO52_AS1_TIE_TAKE=213,222,230,237,276,281 */
#include "types.h"

typedef struct { s16 x, y, z, pad; } Vtx8;
typedef struct { s16 lo, hi; } UV4;

typedef struct {
    Vtx8 v;
    s32 f8;
    u32 col;
    s32 f16;
    s32 f20;
} WorkSlot;

typedef struct {
    u8 pad0[16];
    UV4 uv;
    s32 *ot;
    WorkSlot slot[3];
    u8 tail[72];
    WorkSlot *ptr[3];
} Work;

extern s32 RotAverageNclip3(Vtx8 *, Vtx8 *, Vtx8 *, s32 *, s32 *, s32 *,
                         s32 *, s32 *, s32 *);
extern void ReadSZfifo3(s32 *, s32 *, s32 *);
extern void *RCpolyG3A(void *, Work *, s32, void *);

void *DivideG3(Vtx8 *v0, Vtx8 *v1, Vtx8 *v2, UV4 *uv, u32 *c1,
                    u32 *c2, void *out, s32 *ot, Work *work)
{
    s32 w_;
    s32 flag;
    s32 otz;
    s32 p;
    s32 pad[3];
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
    if (RotAverageNclip3(&p0->v, &p1->v, &p2->v, &p0->f16, &p1->f16,
                      &p2->f16, &otz, &flag, &p) > 0) {
        ReadSZfifo3(&p0->f20, &p1->f20, &p2->f20);
        work->ot = ot;
        work->uv = *uv;
        p0->col = *(u32 *)uv;
        p1->col = *c1;
        p2->col = *c2;
        out = RCpolyG3A(out, work, 0, work->tail);
    }
    return out;
}
