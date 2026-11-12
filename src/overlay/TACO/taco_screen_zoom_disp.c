/* @CompileFlags: -cckr -Wo,-varref,15 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=22:s7 IDO52_AS1_TIE_TAKE=264,273,281,288,294,299,303,307,460,469,477,484,490,495,499,503 */
#include "types.h"

typedef struct {
    u32 tag;
    u8  r0, g0, b0, code;
    s16 x0, y0;
    u8  u0, v0;
    u16 clut;
    s16 x1, y1;
    u8  u1, v1;
    u16 tpage;
    s16 x2, y2;
    u8  u2, v2;
    u16 pad1;
    s16 x3, y3;
    u8  u3, v3;
    u16 pad2;
} PolyFT4;

#define setRGB0(p, r, g, b) (p)->r0 = (r), (p)->g0 = (g), (p)->b0 = (b)
#define setUV4(p, a0, b0, a1, b1, a2, b2, a3, b3) (p)->u0 = (a0), (p)->v0 = (b0), (p)->u1 = (a1), (p)->v1 = (b1), (p)->u2 = (a2), (p)->v2 = (b2), (p)->u3 = (a3), (p)->v3 = (b3)
#define setXY4(p, a0, b0, a1, b1, a2, b2, a3, b3) (p)->x0 = (a0), (p)->y0 = (b0), (p)->x1 = (a1), (p)->y1 = (b1), (p)->x2 = (a2), (p)->y2 = (b2), (p)->x3 = (a3), (p)->y3 = (b3)
#define PCT(v, p) ((v) * (p) / 100)
#define SX(v) (screen_w[activeBuff + 10] + (v))
#define SY(v) (screen_w[activeBuff + 12] + (v))

extern s32 activeBuff;
extern s16 screen_w[];
extern u32 ot_buff0[][256];

extern PolyFT4 *prim_alloc();
extern void SetPolyFT4();
extern void SetShadeTex();
extern void SetSemiTrans();
extern u16 GetTPage();
extern void AddPrim();

taco_screen_zoom_disp(t)
s32 t;
{
    PolyFT4 *s;
    s32 x, h;

    s = prim_alloc(40, activeBuff);
    SetPolyFT4(s);
    SetSemiTrans(s, 0);
    SetShadeTex(s, 0);
    s->tpage = GetTPage(2, 0, 512, 0);
    x = PCT(100 - t, 512) - PCT(100 - t, 16);
    h = PCT(t, 240);
    setRGB0(s, 128, 128, 128);
    setXY4(s, SX(x) - 256, SY(PCT(100 - t, 64)) - 120, SX(x) + PCT(t, 256) - 256, SY(PCT(100 - t, 64)) - 120, SX(x) - 256, SY(PCT(100 - t, 64)) + h - 120, SX(x) + PCT(t, 256) - 256, SY(PCT(100 - t, 64)) + h - 120);
    s->u0 = 0; s->v0 = 0; s->u1 = 255; s->v1 = 0; s->u2 = 0; ((u8 *)s)[29] = 240; s->u3 = 255; ((u8 *)s)[37] = 240;
    AddPrim(&ot_buff0[activeBuff][255], s);

    s = prim_alloc(40, activeBuff);
    SetPolyFT4(s);
    SetSemiTrans(s, 0);
    SetShadeTex(s, 0);
    s->tpage = GetTPage(2, 0, 768, 0);
    x = PCT(100 - t, 256) - PCT(100 - t, 16);
    setRGB0(s, 128, 128, 128);
    setXY4(s, SX(x), SY(PCT(100 - t, 64)) - 120, SX(x) + PCT(t, 256), SY(PCT(100 - t, 64)) - 120, SX(x), SY(PCT(100 - t, 64)) + h - 120, SX(x) + PCT(t, 256), SY(PCT(100 - t, 64)) + h - 120);
    s->u0 = 0; s->v0 = 0; s->u1 = 255; s->v1 = 0; s->u2 = 0; ((u8 *)s)[29] = 240; s->u3 = 255; ((u8 *)s)[37] = 240;
    AddPrim(&ot_buff0[activeBuff][255], s);
}
