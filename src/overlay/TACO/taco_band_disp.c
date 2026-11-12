/* Address: 0x80153E48 */
/* Size: 412 bytes (103 insns) */
#include "types.h"

typedef struct {
    u8 pad0[4];
    u8 f4;
    u8 f5;
    u8 f6;
    u8 f7;
    s16 x0;
    s16 y0;
    s16 x1;
    s16 y1;
    s16 x2;
    s16 y2;
    s16 x3;
    s16 y3;
} TacoQuad;

extern void *prim_alloc(s32, s32);
extern void safe_env(s32);
extern void SetDrawMode(void *, s32, s32, s32, s32);
extern s32 GetTPage(s32, s32, s32, s32);
extern void AddPrim(u8 *, void *);
extern void SetSemiTrans(void *, s32);
extern void SetShadeTex(void *, s32);
extern void SetPolyF4(void *);

extern volatile s32 activeBuff;
extern s16 screen_w[];
extern u8 ot_buff0[];

void taco_band_disp(s32 arg0, u8 arg1, u8 arg2, u8 arg3)
{
    TacoQuad *p;
    void *q;

    p = (TacoQuad *)prim_alloc(24, activeBuff);
    SetPolyF4(p);
    SetSemiTrans(p, 1);
    SetShadeTex(p, 1);

    p->x0 = 0;
    p->y0 = screen_w[activeBuff + 12] - 120;
    p->x1 = 512;
    p->y1 = screen_w[activeBuff + 12] - 120;
    p->x2 = 0;
    p->y2 = screen_w[activeBuff + 12] + 120;
    p->x3 = 512;
    p->y3 = screen_w[activeBuff + 12] + 120;

    p->f4 = arg1;
    p->f5 = arg2;
    p->f6 = arg3;

    AddPrim(&ot_buff0[activeBuff * 1024 + 20], p);

    q = prim_alloc(12, activeBuff);
    SetDrawMode(q, 0, 0, GetTPage(1, arg0, 0, 0), 0);
    AddPrim(&ot_buff0[activeBuff * 1024 + 20], q);
    safe_env(5);
}
