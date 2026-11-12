/* Address: 0x8004B1C4 */
/* Size: 840 bytes (210 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @CompileFlags: -cckr -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: remap_stack_slots --map 88:92 */
#include "types.h"

typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    u8 u0, v0;
    u16 clut;
} SPRT_8;

typedef struct {
    u8 u;
    u8 v;
} UV;

typedef struct {
    u8 pad0[20];
    s16 ofsx[2];
    s16 ofsy[2];
} Disp;

extern u8 _debug_number_show_tbl0;
extern u8 _debug_number_show_str0;
extern u8 _debug_number_show_tbl1[];
extern UV _debug_number_show_tbl2;
extern s32 activeBuff;
extern Disp screen_w;
extern u32 ot_buff0[][256];
extern void SetSemiTrans();
extern void SetSprt8();

void _debug_number_show(num, x, y, pal)
s32 num;
s32 x;
s32 y;
s32 pal;
{
    SPRT_8 *p;
    s32 n;
    s32 i;
    s32 cx;
    s32 cy;

    i = 0;
    if (_debug_number_show_tbl0 < 4 || _debug_number_show_str0 == 0) {
        return;
    }
    n = abs(num);
    while (n >= 0) {
        p = (SPRT_8 *)prim_alloc(16, activeBuff);
        SetSprt8(p);
        SetSemiTrans(p, 0);
        p->r0 = 128;
        p->g0 = 128;
        p->b0 = 128;
        p->x0 = screen_w.ofsx[activeBuff] + x - i * 5;
        p->y0 = screen_w.ofsy[activeBuff] + y;
        p->u0 = _debug_number_show_tbl1[n % 10 * 2];
        p->v0 = _debug_number_show_tbl1[n % 10 * 2 + 1];
        cx = (pal & 8) * 4 + 256;
        cy = (pal & 7) + 504;
        p->clut = GetClut(cx, cy);
        AddPrim(&ot_buff0[activeBuff][1], p);
        if (n < 10) {
            i++;
            break;
        }
        n /= 10;
        i++;
    }
    if (num < 0) {
        p = (SPRT_8 *)prim_alloc(16, activeBuff);
        SetSprt8(p);
        SetSemiTrans(p, 0);
        p->r0 = 128;
        p->g0 = 128;
        p->b0 = 128;
        p->x0 = screen_w.ofsx[activeBuff] + x - i * 5;
        p->y0 = screen_w.ofsy[activeBuff] + y;
        p->u0 = _debug_number_show_tbl2.u + 8;
        p->v0 = _debug_number_show_tbl2.v;
        cx = (pal & 8) * 4 + 256;
        cy = (pal & 7) + 504;
        p->clut = GetClut(cx, cy);
        AddPrim(&ot_buff0[activeBuff][1], p);
    }
    dtd_on_tpage(704, 256, 1, 0, 0);
}
