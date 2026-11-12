/* @CompileFlags: -Wc,-pic0 */
/* @PostProc: remap_stack_slots --map '122:118,123:119,124:120,125:121,126:122,127:123,116:112' */
/* Address: 0x80144DEC */
/* Size: 1888 bytes (472 instructions) */
#include "types.h"

typedef struct {
      u32 tag;
      u8 r0;
      u8 g0;
      u8 b0;
      u8 code;
      s16 x0;
      s16 y0;
      u8 r1;
      u8 g1;
      u8 b1;
      u8 p1;
      s16 x1;
      s16 y1;
      u8 r2;
      u8 g2;
      u8 b2;
      u8 p2;
      s16 x2;
      s16 y2;
      u8 r3;
      u8 g3;
      u8 b3;
      u8 p3;
      s16 x3;
      s16 y3;
} Poly;

extern s16 screen_w[];
extern u8 ot_buff0[];
extern s32 activeBuff;

extern Poly *prim_alloc(s32, s32);
extern void safe_env(s32);
extern void SetDrawMode(s32, s32, s32, s32, s32);
extern s32 GetTPage(s32, s32, s32, s32);
extern void AddPrim(u8 *, void *);
extern void SetSemiTrans(Poly *, s32);
extern void SetShadeTex(Poly *, s32);
extern void SetPolyG4(Poly *);
extern void taco_quad_spin(Poly **, s32, s32);

void
taco_grad_disp(s32 x, s32 y, s32 h, s32 otx, volatile u8 lum, s32 mode, s32 ang)
{
    Poly *obj[4];
    u8 rb;
    u8 gb;
    u8 bb;
    u8 ra;
    u8 ga;
    u8 ba;
    s32 i;
    Poly *p;
    s32 *ot;
    s32 q2;
    s32 q3;
    s32 ofs;
    s32 lv;

    lv = lum;
    for (i = 0; i < 3; i++) {
        p = prim_alloc(36, activeBuff);
        obj[i] = p;
        SetPolyG4(p);
        SetSemiTrans(obj[i], 1);
        SetShadeTex(obj[i], 1);

        switch (mode) {
        case 0:
            bb = 255;
            gb = 255;
            rb = 255;
            ga = 32;
            ra = 32;
            ba = 80;
            if (lv < 248U) {
                rb = lv + 8;
                gb = lv + 8;
            }
            if (lv < 240U) {
                bb = lv + 16;
            }
            if (lv < 24U) {
                ra = lv + 8;
                ga = lv + 8;
            }
            if (lv < 64U) {
                ba = lv + 16;
            }
            break;
        case 1:
            ga = 8;
            ra = 8;
            gb = 8;
            rb = 8;
            ba = 16;
            bb = 16;
            if (lv < 248U) {
                rb = 255 - lv;
                gb = 255 - lv;
            }
            if (lv < 240U) {
                bb = 255 - lv;
            }
            if (lv < 24U) {
                ra = 32 - lv;
                ga = 32 - lv;
            }
            if (lv < 64U) {
                ba = 80 - lv;
            }
            break;
        case 2:
            bb = 255;
            gb = 255;
            rb = 255;
            ga = 32;
            ra = 32;
            ba = 80;
            break;
        }

        switch (i) {
        case 0:
            q3 = h / 3;
            obj[i]->x0 = (s16)x;
            obj[i]->y0 = (s16)(screen_w[activeBuff + 12] + y);
            obj[i]->x1 = (s16)(x + 320);
            obj[i]->y1 = (s16)(screen_w[activeBuff + 12] + y);
            obj[i]->x2 = (s16)x;
            obj[i]->y2 = (s16)(screen_w[activeBuff + 12] + y + q3);
            obj[i]->x3 = (s16)(x + 320);
            obj[i]->y3 = (s16)(screen_w[activeBuff + 12] + y + q3);
            obj[i]->r0 = ra;
            obj[i]->g0 = ga;
            obj[i]->b0 = ba;
            obj[i]->r1 = ra;
            obj[i]->g1 = ga;
            obj[i]->b1 = ba;
            obj[i]->r2 = rb;
            obj[i]->g2 = gb;
            obj[i]->b2 = bb;
            obj[i]->r3 = rb;
            obj[i]->g3 = gb;
            obj[i]->b3 = bb;
            break;
        case 1:
            q3 = h / 3;
            q2 = h / 2;
            obj[i]->x0 = (s16)x;
            obj[i]->y0 = (s16)(screen_w[activeBuff + 12] + y + q3 + q2);
            obj[i]->x1 = (s16)(x + 320);
            obj[i]->y1 = (s16)(screen_w[activeBuff + 12] + y + q3 + q2);
            obj[i]->x2 = (s16)x;
            obj[i]->y2 =
                (s16)(screen_w[activeBuff + 12] + y + q3 + q2 + q3);
            obj[i]->x3 = (s16)(x + 320);
            obj[i]->y3 =
                (s16)(screen_w[activeBuff + 12] + y + q3 + q2 + q3);
            obj[i]->r0 = rb;
            obj[i]->g0 = gb;
            obj[i]->b0 = bb;
            obj[i]->r1 = rb;
            obj[i]->g1 = gb;
            obj[i]->b1 = bb;
            obj[i]->r2 = ra;
            obj[i]->g2 = ga;
            obj[i]->b2 = ba;
            obj[i]->r3 = ra;
            obj[i]->g3 = ga;
            obj[i]->b3 = ba;
            break;
        case 2:
            q3 = h / 3;
            q2 = h / 2;
            obj[i]->x0 = (s16)x;
            obj[i]->y0 = (s16)(screen_w[activeBuff + 12] + y + q3);
            obj[i]->x1 = (s16)(x + 320);
            obj[i]->y1 = (s16)(screen_w[activeBuff + 12] + y + q3);
            obj[i]->x2 = (s16)x;
            obj[i]->y2 = (s16)(screen_w[activeBuff + 12] + y + q3 + q2);
            obj[i]->x3 = (s16)(x + 320);
            obj[i]->y3 = (s16)(screen_w[activeBuff + 12] + y + q3 + q2);
            obj[i]->r0 = rb;
            obj[i]->g0 = gb;
            obj[i]->b0 = bb;
            obj[i]->r1 = rb;
            obj[i]->g1 = gb;
            obj[i]->b1 = bb;
            obj[i]->r2 = rb;
            obj[i]->g2 = gb;
            obj[i]->b2 = bb;
            obj[i]->r3 = rb;
            obj[i]->g3 = gb;
            obj[i]->b3 = bb;
            break;
        }
    }

    if (ang > 0) {
        taco_quad_spin(obj, ang, h);
    }

    ofs = otx * 4;
    for (i = 0; i < 3; i++) {
        AddPrim(&ot_buff0[(activeBuff << 10) + ofs], obj[i]);
    }

    ot = (s32 *)prim_alloc(12, activeBuff);
    SetDrawMode((s32)ot, 0, 0, GetTPage(1, 1, 0, 0), 0);
    AddPrim(&ot_buff0[(activeBuff << 10) + ofs], ot);
    safe_env(otx);
}
