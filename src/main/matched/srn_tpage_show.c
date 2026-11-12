/* Address: 0x8004C750 */
/* Size: 3224 bytes (806 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @CompileFlags: -cckr -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_insn_reg_field 0x120:rs:t7:t6 0x120:rt:t6:t7 0x130:rs:t9:t6 0x130:rt:t6:t9 0x644:rs:t7:v0 0x644:rt:v0:t7 0x654:rs:t6:v0 0x654:rt:v0:t6 0xac8:rs:t8:v0 0xac8:rt:v0:t8 0xadc:rs:t5:v0 0xadc:rt:v0:t5 */
#include "types.h"

typedef struct {
    u8 pad0[4];
    u8 r, g, b;
    u8 pad7;
    s16 x0, y0;
    u8 u0, v0;
    s16 clut;
    s16 x1, y1;
    u8 u1, v1;
    s16 tpage;
    s16 x2, y2;
    u8 u2, v2;
    s16 pad30;
    s16 x3, y3;
    u8 u3, v3;
} Spr;

extern Spr *prim_alloc();
extern void SetPolyFT4();
extern void SetSemiTrans();
extern void SetShadeTex();
extern void AddPrim();
extern s16 GetTPage(s32, s32, s32, s32);

#define setUV4(p, _u0, _v0, _u1, _v1, _u2, _v2, _u3, _v3) (p)->u0 = _u0, (p)->v0 = _v0, (p)->u1 = _u1, (p)->v1 = _v1, (p)->u2 = _u2, (p)->v2 = _v2, (p)->u3 = _u3, (p)->v3 = _v3
extern u8 screen_bri;
typedef struct { u8 pad0[20]; s16 ofsx[2]; s16 ofsy[2]; } Disp;
extern Disp screen_w;
extern s32 activeBuff;
extern u32 ot_buff0[][256];

void srn_tpage_show(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)
u8 arg0; u8 arg1; u8 arg2; s16 *arg3; s32 arg4; s32 arg5; s16 arg6; s16 arg7; s32 arg8;
{
    Disp *sv;
    Spr *obj;
    s32 divisor;
    s32 width;
    switch (arg1) {
    case 0:
        divisor = 4; width = 64;
        break;
    case 1:
        divisor = 2; width = 128;
        break;
    case 2:
        divisor = 1; width = 256;
        break;
    }

    obj = prim_alloc(40, activeBuff);
    SetPolyFT4(obj);
    if (arg2) {
        SetSemiTrans(obj, 1);
    } else {
        SetSemiTrans(obj, 0);
    }
    SetShadeTex(obj, 0);
    obj->r = screen_bri;
    obj->g = screen_bri;
    obj->b = screen_bri;

    setUV4(obj, 0, ((arg0 & 0x20) << 2), 255, ((arg0 & 0x20) << 2), 0, ((arg0 & 0x20) << 2) + arg3[3], 255, ((arg0 & 0x20) << 2) + arg3[3]);
    sv = &screen_w;

    obj->x0 = arg3[0] + (sv->ofsx[activeBuff] + (-(arg4 & 0xff)));
    obj->y0 = arg3[1] + sv->ofsy[activeBuff];
    obj->x1 = arg3[0] + (sv->ofsx[activeBuff] + (-(arg4 & 0xff))) + 255;
    obj->y1 = arg3[1] + sv->ofsy[activeBuff];
    obj->x2 = arg3[0] + (sv->ofsx[activeBuff] + (-(arg4 & 0xff)));
    obj->y2 = arg3[3] + (arg3[1] + sv->ofsy[activeBuff]);
    obj->x3 = arg3[0] + (sv->ofsx[activeBuff] + (-(arg4 & 0xff))) + 255;
    obj->y3 = arg3[3] + (arg3[1] + sv->ofsy[activeBuff]);
    obj->tpage = GetTPage(arg1, arg2 & 3, ((arg0 & 0xf) << 6) + (arg4 & 0x7f00) / divisor, ((arg0 & 0x10) << 4) + ((arg0 & 0x20) << 2));
    obj->clut = GetClut(arg6, arg7);
    AddPrim(&ot_buff0[activeBuff][arg8], obj);

    obj = prim_alloc(40, activeBuff);
    SetPolyFT4(obj);
    if (arg2) {
        SetSemiTrans(obj, 1);
    } else {
        SetSemiTrans(obj, 0);
    }
    SetShadeTex(obj, 0);
    obj->r = screen_bri;
    obj->g = screen_bri;
    obj->b = screen_bri;

    setUV4(obj, (arg4 / 256) * 4 + 3, 0, (arg4 / 256) * 4 + 4, 0, (arg4 / 256) * 4 + 3, arg3[3], (arg4 / 256) * 4 + 4, arg3[3]);

    obj->x0 = arg3[0] + (sv->ofsx[activeBuff] + (255 - (arg4 & 0xff)));
    obj->y0 = arg3[1] + sv->ofsy[activeBuff];
    obj->x1 = arg3[0] + (sv->ofsx[activeBuff] + (255 - (arg4 & 0xff))) + 1;
    obj->y1 = arg3[1] + sv->ofsy[activeBuff];
    obj->x2 = arg3[0] + (sv->ofsx[activeBuff] + (255 - (arg4 & 0xff)));
    obj->y2 = arg3[3] + (arg3[1] + sv->ofsy[activeBuff]);
    obj->x3 = arg3[0] + (sv->ofsx[activeBuff] - (arg4 & 0xff)) + 256;
    obj->y3 = arg3[3] + (arg3[1] + sv->ofsy[activeBuff]);
    obj->tpage = GetTPage(arg1, arg2 & 3, 960, 0);
    obj->clut = GetClut(arg6, arg7);
    AddPrim(&ot_buff0[activeBuff][arg8 - 1], obj);

    obj = prim_alloc(40, activeBuff);
    SetPolyFT4(obj);
    if (arg2) {
        SetSemiTrans(obj, 1);
    } else {
        SetSemiTrans(obj, 0);
    }
    SetShadeTex(obj, 0);
    obj->r = screen_bri;
    obj->g = screen_bri;
    obj->b = screen_bri;
    setUV4(obj, 0, ((arg0 & 0x20) << 2), 255, ((arg0 & 0x20) << 2), 0, ((arg0 & 0x20) << 2) + arg3[3], 255, ((arg0 & 0x20) << 2) + arg3[3]);
    obj->x0 = arg3[0] + (sv->ofsx[activeBuff] + (-(arg4 & 0xff))) + 256;
    obj->y0 = arg3[1] + sv->ofsy[activeBuff];
    obj->x1 = arg3[0] + (sv->ofsx[activeBuff] + (-(arg4 & 0xff))) + 511;
    obj->y1 = arg3[1] + sv->ofsy[activeBuff];
    obj->x2 = arg3[0] + (sv->ofsx[activeBuff] + (-(arg4 & 0xff))) + 256;
    obj->y2 = arg3[3] + (arg3[1] + sv->ofsy[activeBuff]);
    obj->x3 = arg3[0] + (sv->ofsx[activeBuff] + (-(arg4 & 0xff))) + 511;
    obj->y3 = arg3[3] + (arg3[1] + sv->ofsy[activeBuff]);
    obj->tpage = GetTPage(arg1, arg2 & 3, ((arg0 & 0xf) << 6) + (arg4 & 0x7f00) / divisor + width, ((arg0 & 0x10) << 4) + ((arg0 & 0x20) << 2));
    obj->clut = GetClut(arg6, arg7);
    AddPrim(&ot_buff0[activeBuff][arg8], obj);

    obj = prim_alloc(40, activeBuff);
    SetPolyFT4(obj);
    if (arg2) {
        SetSemiTrans(obj, 1);
    } else {
        SetSemiTrans(obj, 0);
    }
    SetShadeTex(obj, 0);
    obj->r = screen_bri;
    obj->g = screen_bri;
    obj->b = screen_bri;
    setUV4(obj, (arg4 / 256) * 4 + 7, 0, (arg4 / 256) * 4 + 8, 0, (arg4 / 256) * 4 + 7, arg3[3], (arg4 / 256) * 4 + 8, arg3[3]);
    obj->x0 = arg3[0] + (sv->ofsx[activeBuff] + (255 - (arg4 & 0xff))) + 256;
    obj->y0 = arg3[1] + sv->ofsy[activeBuff];
    obj->x1 = arg3[0] + (sv->ofsx[activeBuff] + (255 - (arg4 & 0xff))) + 257;
    obj->y1 = arg3[1] + sv->ofsy[activeBuff];
    obj->x2 = arg3[0] + (sv->ofsx[activeBuff] + (255 - (arg4 & 0xff))) + 256;
    obj->y2 = arg3[3] + (arg3[1] + sv->ofsy[activeBuff]);
    obj->x3 = arg3[0] + (sv->ofsx[activeBuff] - (arg4 & 0xff)) + 512;
    obj->y3 = arg3[3] + (arg3[1] + sv->ofsy[activeBuff]);
    obj->tpage = GetTPage(arg1, arg2 & 3, 960, 0);
    obj->clut = GetClut(arg6, arg7);
    AddPrim(&ot_buff0[activeBuff][arg8], obj);

    obj = prim_alloc(40, activeBuff);
    SetPolyFT4(obj);
    if (arg2) {
        SetSemiTrans(obj, 1);
    } else {
        SetSemiTrans(obj, 0);
    }
    SetShadeTex(obj, 0);
    obj->r = screen_bri;
    obj->g = screen_bri;
    obj->b = screen_bri;
    setUV4(obj, 0, ((arg0 & 0x20) << 2), 255, ((arg0 & 0x20) << 2), 0, ((arg0 & 0x20) << 2) + arg3[3], 255, ((arg0 & 0x20) << 2) + arg3[3]);
    obj->x0 = arg3[0] + (sv->ofsx[activeBuff] + (-(arg4 & 0xff))) + 512;
    obj->y0 = arg3[1] + sv->ofsy[activeBuff];
    obj->x1 = arg3[0] + (sv->ofsx[activeBuff] + (-(arg4 & 0xff))) + 767;
    obj->y1 = arg3[1] + sv->ofsy[activeBuff];
    obj->x2 = arg3[0] + (sv->ofsx[activeBuff] + (-(arg4 & 0xff))) + 512;
    obj->y2 = arg3[3] + (arg3[1] + sv->ofsy[activeBuff]);
    obj->x3 = arg3[0] + (sv->ofsx[activeBuff] + (-(arg4 & 0xff))) + 767;
    obj->y3 = arg3[3] + (arg3[1] + sv->ofsy[activeBuff]);
    obj->tpage = GetTPage(arg1, arg2 & 3, ((arg0 & 0xf) << 6) + (arg4 & 0x7f00) / divisor + width * 2, ((arg0 & 0x10) << 4) + ((arg0 & 0x20) << 2));
    obj->clut = GetClut(arg6, arg7);
    AddPrim(&ot_buff0[activeBuff][arg8], obj);
}
