/* Address: 0x8004F238 */
/* Size: 1112 bytes (278 instructions) */
/* @SymbolStart: 0x8004F238 */
/* @SymbolSize: 1112 */
/* @CompileFlags: -Wc,-pic1 */
/* @CompileEnv: IDO52_COPYPROP_VAR=-48 IDO52_COPYPROP_SITE=lod:1:deny IDO52_AS1_TIE_REJECT=231 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    s16 x;
    s16 y;
    u8 tp;
    u8 count;
    u8 pal;
    s8 flag;
} IconRec;

extern IconRec disp_string_tbl0[];
extern u8 disp_string_tbl1;
extern s16 check_end_k_tbl0;
extern s16 check_end_k_tbl1;
extern u8 k_cell_w;
extern s32 activeBuff;
extern u8 screen_w[];
extern u8 screen_bri;
extern u8 ot_buff0[];

extern u8 *prim_alloc(s32 size, s32 idx);
extern void SetSprt16(u8 *p);
extern void SetSemiTrans(u8 *p, s32 on);
extern void SetShadeTex(u8 *p, s32 on);
extern u16 GetClut(s32 x, s32 y);
extern void AddPrim(u8 *ot, u8 *p);
extern void goto_tpage(s32 a, s32 b, s32 c, s32 d, s32 e);

void disp_string(void)
{
    IconRec *r;
    u8 *p;
    s32 pal;
    s32 i;
    s32 col;
    s32 first;
    s32 palOff;
    s32 u;
    s32 v;
    s32 idx;
    u32 tp;
    s32 k;

    pal = disp_string_tbl1;
    k = 0;
    if (pal == 0) {
        pal = 1;
    }
    first = 1;
    i = 0;
    col = 0;
    if (check_end_k_tbl0 > 0 && check_end_k_tbl1 > 0) {
        r = &disp_string_tbl0[k];
        do {
            if (!((u32)col < r->count)) {
                col = 0;
                r++;
            }
            if (r->flag == 1) {
                if (pal != r->pal) {
                    pal = r->pal;
                    first = 1;
                    if (pal == 0) {
                        pal = 1;
                    }
                }
                p = prim_alloc(16, activeBuff);
                palOff = pal << 2;
                SetSprt16(p);
                SetSemiTrans(p, 0);
                SetShadeTex(p, 0);
                p[4] = screen_bri;
                p[5] = screen_bri;
                p[6] = screen_bri;
                *(s16 *)(p + 8) = *(s16 *)(screen_w + 20 + activeBuff * 2) + r->x + col * k_cell_w;
                u = ((i & 7) << 4) + 128;
                v = (i & 0x78) << 1;
                *(s16 *)(p + 10) = *(s16 *)(screen_w + 24 + activeBuff * 2) + r->y;
                p[12] = u;
                p[13] = v;
                tp = r->tp;
                *(u16 *)(p + 14) = GetClut(((tp & 8) << 2) + 256, (tp & 7) + 504);
                AddPrim(ot_buff0 + (activeBuff << 10) + palOff, p);

                p = prim_alloc(16, activeBuff);
                SetSprt16(p);
                SetSemiTrans(p, 0);
                SetShadeTex(p, 0);
                p[4] = 0;
                p[5] = 0;
                p[6] = 0;
                *(s16 *)(p + 8) = *(s16 *)(screen_w + 20 + activeBuff * 2) + r->x + col * k_cell_w + 1;
                *(s16 *)(p + 10) = *(s16 *)(screen_w + 24 + activeBuff * 2) + r->y + 1;
                p[13] = v;
                p[12] = u;
                tp = r->tp;
                *(u16 *)(p + 14) = GetClut(((tp & 8) << 2) + 272, (tp & 7) + 504);
                AddPrim(ot_buff0 + (activeBuff << 10) + palOff, p);

                if (i == 127) {
                    goto_tpage(768, 256, pal, 0, 0);
                    first = 0;
                } else if (i == 255) {
                    goto_tpage(832, 256, pal, 0, 0);
                    first = 0;
                } else if (first != 0) {
                    first = 0;
                    if (i >= 256) {
                        goto_tpage(896, 256, pal, 0, 0);
                    } else if (i >= 128) {
                        goto_tpage(832, 256, pal, 0, 0);
                    } else {
                        goto_tpage(768, 256, pal, 0, 0);
                    }
                }
            }
            i++;
            col++;
        } while (i < check_end_k_tbl0 && i < check_end_k_tbl1);
    }
    if (i >= 256) {
        goto_tpage(896, 256, pal, 0, 0);
    } else if (i >= 128) {
        goto_tpage(832, 256, pal, 0, 0);
    } else {
        goto_tpage(768, 256, pal, 0, 0);
    }
}
