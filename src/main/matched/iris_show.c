/* Function: iris_show -- textured quad burst + optional edge mask tiles */
/* Address: 0x80046E08 */
/* Size: 2684 bytes (671 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolSize: 2684 */
/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_COPYPROP_SITE=lod:4:deny,lod:5:deny,lod:6:deny,lod:7:deny IDO52_COPYPROP_VAR=-44 IDO52_FORCE_WEB_COLOR=11:s7,203:a1,126:a0 IDO52_UGEN_DEST_TEMP=26,241,243 IDO52_AS1_NO_COPY_PROP=30 IDO52_AS1_KEEP_DEAD_RELOAD=3 IDO52_UGEN_TEMP_REG=10:t6,11:t7,12:t8,13:t9,14:t0,16:t6,17:t7,18:t8,19:t9,20:t0,205:t6,206:t7,207:t8,208:t0,209:t3,211:t6,212:t7,213:t8,215:t0,217:t4 IDO52_AS1_TIE_REJECT=2916,2919 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: remap_stack_slots --map '76:88,80:92,84:100,88:104,92:112,96:124' */
/* @PostProc: patch_insn_reg_field 0xec:rt:v1:t4 0xf4:rt:v1:t4 0xfc:rd:v0:t3 0xfc:rt:v1:t4 0x100:rt:v0:t3 0x108:rt:v1:t4 0x10c:rt:v0:t3 0x114:rt:v1:t4 0x118:rt:v0:t3 0x120:rt:v1:t4 0x124:rt:v0:t3 0x130:rt:v1:t4 */
#include "types.h"

typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    u8 u0, v0;
    u16 clut;
    u8 r1, g1, b1, p1;
    s16 x1, y1;
    u8 u1, v1;
    u16 tpage;
    u8 r2, g2, b2, p2;
    s16 x2, y2;
    u8 u2, v2;
    u16 pad2;
    u8 r3, g3, b3, p3;
    s16 x3, y3;
    u8 u3, v3;
    u16 pad3;
} POLY_GT4;

typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    s16 w, h;
} TILE;

typedef struct {
    u8 pad0[20];
    s16 ofsx[2];
    s16 ofsy[2];
} Disp;

extern s32 activeBuff;
extern Disp screen_w;
extern u32 ot_buff0[][256];

#define setRGB0(p, r, g, b) (p)->r0 = r, (p)->g0 = g, (p)->b0 = b
#define setRGB1(p, r, g, b) (p)->r1 = r, (p)->g1 = g, (p)->b1 = b
#define setRGB2(p, r, g, b) (p)->r2 = r, (p)->g2 = g, (p)->b2 = b
#define setRGB3(p, r, g, b) (p)->r3 = r, (p)->g3 = g, (p)->b3 = b
#define setUV4(p, _u0, _v0, _u1, _v1, _u2, _v2, _u3, _v3) (p)->u0 = _u0, (p)->v0 = _v0, (p)->u1 = _u1, (p)->v1 = _v1, (p)->u2 = _u2, (p)->v2 = _v2, (p)->u3 = _u3, (p)->v3 = _v3

void iris_show(mode, cx, cy, size, x, y, color, pri)
s32 mode;
s32 cx;
s32 cy;
s32 size;
s32 x;
s32 y;
s32 color;
s32 pri;
{
    Disp *sv;
    POLY_GT4 *p;
    TILE *t;
    s32 i;
    s32 v;
    s32 tx;
    s32 ty;
    s32 semi;
    s32 z;
    s32 h;
    s32 r;
    s32 g;
    s32 b;
    s32 zofs;
    s32 unused0;
    s32 unused1;

    z = pri & 0xf;
    v = (mode & 0x20) << 2;
    tx = (mode & 0xf) << 6;
    ty = ((mode & 0x20) << 2) + ((mode & 0x10) << 4);
    zofs = z * 4;
    semi = (color >> 28) & 0xf;
    sv = &screen_w;
    for (i = 0; i < 4; i++) {
        p = (POLY_GT4 *)prim_alloc(52, activeBuff);
        SetPolyGT4(p);
        if (semi) {
            SetSemiTrans(p, 1);
        } else {
            SetSemiTrans(p, 0);
        }
        if (semi) {
            r = (color >> 16) & 0xff;
            setRGB0(p, r, color >> 8, color);
            setRGB1(p, r, color >> 8, color);
            setRGB2(p, r, color >> 8, color);
            setRGB3(p, r, color >> 8, color);
        } else {
            setRGB0(p, 128, 128, 128);
            setRGB1(p, 128, 128, 128);
            setRGB2(p, 128, 128, 128);
            setRGB3(p, 128, 128, 128);
        }
        switch (i) {
        case 0:
            p->x0 = sv->ofsx[activeBuff] + x;
            p->y0 = sv->ofsy[activeBuff] + y;
            p->x1 = sv->ofsx[activeBuff] + x + size * 128 / 4096;
            p->y1 = sv->ofsy[activeBuff] + y;
            p->x2 = sv->ofsx[activeBuff] + x;
            p->y2 = sv->ofsy[activeBuff] + y + size * 128 / 4096;
            p->x3 = sv->ofsx[activeBuff] + x + size * 128 / 4096;
            p->y3 = sv->ofsy[activeBuff] + y + size * 128 / 4096;
            setUV4(p, 0, v, 127, v, 0, v + 127, 127, v + 127);
            break;
        case 1:
            p->x0 = sv->ofsx[activeBuff] + x;
            p->y0 = sv->ofsy[activeBuff] + y;
            p->x1 = sv->ofsx[activeBuff] + x + size * 128 / 4096;
            p->y1 = sv->ofsy[activeBuff] + y;
            p->x2 = sv->ofsx[activeBuff] + x;
            p->y2 = sv->ofsy[activeBuff] + y + -(size * 128 / 4096);
            p->x3 = sv->ofsx[activeBuff] + x + size * 128 / 4096;
            p->y3 = sv->ofsy[activeBuff] + y + -(size * 128 / 4096);
            setUV4(p, 0, v, 127, v, 0, v + 127, 127, v + 127);
            break;
        case 2:
            p->x0 = sv->ofsx[activeBuff] + x;
            p->y0 = sv->ofsy[activeBuff] + y;
            p->x1 = sv->ofsx[activeBuff] + x + -(size * 128 / 4096);
            p->y1 = sv->ofsy[activeBuff] + y;
            p->x2 = sv->ofsx[activeBuff] + x;
            p->y2 = sv->ofsy[activeBuff] + y + size * 128 / 4096;
            p->x3 = sv->ofsx[activeBuff] + x + -(size * 128 / 4096);
            p->y3 = sv->ofsy[activeBuff] + y + size * 128 / 4096;
            setUV4(p, 0, v, 127, v, 0, v + 127, 127, v + 127);
            break;
        case 3:
            p->x0 = sv->ofsx[activeBuff] + x;
            p->y0 = sv->ofsy[activeBuff] + y;
            p->x1 = sv->ofsx[activeBuff] + x + -(size * 128 / 4096);
            p->y1 = sv->ofsy[activeBuff] + y;
            p->x2 = sv->ofsx[activeBuff] + x;
            p->y2 = sv->ofsy[activeBuff] + y + -(size * 128 / 4096);
            p->x3 = sv->ofsx[activeBuff] + x + -(size * 128 / 4096);
            p->y3 = sv->ofsy[activeBuff] + y + -(size * 128 / 4096);
            setUV4(p, 0, v, 127, v, 0, v + 127, 127, v + 127);
            break;
        }
        p->tpage = GetTPage(0, 0, tx, ty);
        p->clut = GetClut(cx, cy);
        AddPrim((u32 *)((u8 *)ot_buff0[activeBuff] + zofs), p);
        dtd_on_tpage(0, 0, z, 1, 0);
    }
    if ((color >> 24) & 0xf) {
        r = (color >> 16) & 0xff;
        g = (color >> 8) & 0xff;
        b = color & 0xff;
        for (i = 0; i < 4; i++) {
            t = (TILE *)prim_alloc(16, activeBuff);
            SetTile(t);
            SetSemiTrans(t, 0);
            SetShadeTex(t, 0);
            setRGB0(t, r, g, b);
            switch (i) {
            case 0:
                t->x0 = sv->ofsx[activeBuff] - 160;
                t->y0 = sv->ofsy[activeBuff] - 120;
                t->w = 320;
                h = y - size * 128 / 4096 + 120;
                t->h = h;
                if (h > 0) {
                    AddPrim(&ot_buff0[activeBuff][z], t);
                }
                break;
            case 1:
                t->x0 = sv->ofsx[activeBuff] - 160;
                t->y0 = sv->ofsy[activeBuff] - 120;
                h = x - size * 128 / 4096 + 160;
                t->w = h;
                t->h = 240;
                if (h > 0) {
                    AddPrim(&ot_buff0[activeBuff][z], t);
                }
                break;
            case 2:
                t->x0 = sv->ofsx[activeBuff] - 160;
                t->y0 = sv->ofsy[activeBuff] + y + size * 128 / 4096;
                h = -y - size * 128 / 4096 + 120;
                t->h = h;
                t->w = 320;
                if (h > 0) {
                    AddPrim(&ot_buff0[activeBuff][z], t);
                }
                break;
            case 3:
                t->x0 = sv->ofsx[activeBuff] + x + size * 128 / 4096;
                t->y0 = sv->ofsy[activeBuff] - 120;
                h = -x - size * 128 / 4096 + 160;
                t->w = h;
                t->h = 240;
                if (h > 0) {
                    AddPrim(&ot_buff0[activeBuff][z], t);
                }
                break;
            }
        }
    }
}
