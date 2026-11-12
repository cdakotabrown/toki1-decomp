/* Address: 0x800AFAC8, Size: 620 bytes (155 instructions; the 3 zero words to */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: remap_stack_slots --map '52:84,76:88,80:92,84:96,88:100' */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=2:v1,4:t0,8:t1,12:t2,16:t3,22:t4,25:t5,53:t4,59:t5 IDO52_FORCE_WEB_COLOR_NOSPLIT=22,25 IDO52_AS1_NO_COPY_PROP=all IDO52_AS1_NO_PASS1_PROP=all IDO52_AS1_NO_LA_PULL=all IDO52_AS1_KEEP_DEAD_RELOAD=0 IDO52_AS1_TIE_TAKE=134,142,150,157,163,168,178,202,209,215 IDO52_UGEN_TEMP_REG=2:t6,4:t7,7:t9,10:t7,11:t9,13:t7,21:t8 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

typedef struct { u8 b[8]; } Pad8;
typedef struct { u8 b[4]; } Pad4;

typedef struct {
    Pad8 v;
    s32  w8;
    s32  w12;
    s32  w16;
    s32  w20;
} Rec;

typedef struct {
    u8   pad0[12];
    s16  h12;
    s16  h14;
    Pad4 p16;
    s32  w20;
    Rec  rec[4];
    u8   pad120[120];
    Rec *ptr[4];
} Work;

extern s32 RotAverageNclip4(Rec *, Rec *, Rec *, Rec *, Rec *, Rec *, Rec *, Rec *,
                         s32 *, s32 *, s32 *);
extern void ReadSZfifo4(s32 *, s32 *, s32 *, s32 *);
extern s32 RCpolyGT4A(s32, Work *, s32, void *);

s32 DivideGT4(Pad8 *p0, Pad8 *p1, Pad8 *p2, Pad8 *p3,
                  u32 *q4, u32 *q5, s32 *q6, s32 *q7,
                  s32 *r8, s32 *r9, s32 *r10, s32 *r11,
                  s32 a12, s32 a13, Work *w)
{
    s32 w_;
    s32 l144;
    s32 l140;
    s32 l136;
    s32 pad[7];
    Rec *s0;
    Rec *s1;
    Rec *s2;
    Rec *s3;

    s0 = &w->rec[0];
    s1 = &w->rec[1];
    s2 = &w->rec[2];
    s3 = &w->rec[3];
    w->ptr[0] = s0;
    w->ptr[1] = s1;
    w->ptr[2] = s2;
    w->ptr[3] = s3;
    s0->v = *p0;
    s1->v = *p1;
    s2->v = *p2;
    s3->v = *p3;

    if (RotAverageNclip4(s0, s1, s2, s3,
                      (Rec *)&s0->w16, (Rec *)&s1->w16,
                      (Rec *)&s2->w16, (Rec *)&s3->w16,
                      &l140, &l144, &l136) > 0) {
        ReadSZfifo4(&s0->w20, &s1->w20, &s2->w20, &s3->w20);
        w->w20 = a13;
        w->p16 = *(Pad4 *)r8;
        w->h12 = *q4 >> 16;
        w->h14 = *q5 >> 16;
        s0->w8 = *q4;
        s1->w8 = *q5;
        s2->w8 = *q6;
        s3->w8 = *q7;
        s0->w12 = *r8;
        s1->w12 = *r9;
        s2->w12 = *r10;
        s3->w12 = *r11;
        a12 = RCpolyGT4A(a12, w, 0, (u8 *)w + 120);
    }
    return a12;
}
