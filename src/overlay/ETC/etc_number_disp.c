/* @SymbolSize: 796 */
/* @PostProc: patch_stack_complete --old 104 --new 120 */
/* @PostProc: remap_stack_slots --map '72:92' */
/* @PostProc: swap_instructions --offset1 0x7c --offset2 0x80 */
/* @PostProc: swap_instructions --offset1 0x80 --offset2 0x84 */
/* @PostProc: swap_instructions --offset1 0x84 --offset2 0x88 */
/* @PostProc: swap_instructions --offset1 0x208 --offset2 0x20c */
/* @PostProc: swap_instructions --offset1 0x204 --offset2 0x208 */
/* @PostProc: swap_instructions --offset1 0x25c --offset2 0x260 */
/* @PostProc: swap_instructions --offset1 0x258 --offset2 0x25c */
/* @PostProc: swap_instructions --offset1 0x254 --offset2 0x258 */
/* @PostProc: swap_instructions --offset1 0x250 --offset2 0x254 */
/* @PostProc: swap_instructions --offset1 0x24c --offset2 0x250 */
/* @PostProc: swap_instructions --offset1 0x248 --offset2 0x24c */
/* @PostProc: swap_instructions --offset1 0x244 --offset2 0x248 */
/* @PostProc: swap_instructions --offset1 0x240 --offset2 0x244 */
/* @PostProc: swap_instructions --offset1 0x23c --offset2 0x240 */
/* @PostProc: swap_instructions --offset1 0x238 --offset2 0x23c */
/* @PostProc: swap_instructions --offset1 0x234 --offset2 0x238 */
/* @PostProc: swap_instructions --offset1 0x230 --offset2 0x234 */
/* @PostProc: swap_instructions --offset1 0x22c --offset2 0x230 */
/* @PostProc: swap_instructions --offset1 0x228 --offset2 0x22c */
/* @PostProc: swap_instructions --offset1 0x224 --offset2 0x228 */
/* @PostProc: swap_instructions --offset1 0x220 --offset2 0x224 */
/* @PostProc: swap_instructions --offset1 0x21c --offset2 0x220 */
/* @PostProc: swap_instructions --offset1 0x218 --offset2 0x21c */
/* @PostProc: swap_instructions --offset1 0x214 --offset2 0x218 */
/* @PostProc: swap_instructions --offset1 0x26c --offset2 0x270 */
/* @PostProc: swap_instructions --offset1 0x268 --offset2 0x26c */
/* @PostProc: swap_instructions --offset1 0x264 --offset2 0x268 */
/* @PostProc: patch_insn_reg_field 0x68:rd:s6:s5 0x8c:rt:s5:s6 0xb4:rt:s5:s6 0xb8:rt:s5:s6 0x1ac:rs:s6:s5 0x1ac:rt:s6:s5 0x1f8:rt:s5:s6 0x204:rs:s6:s5 0x204:rt:s6:s5 0x214:rd:v0:s2 0x214:rt:s6:s5 0x22c:rt:s5:s6 0x230:rt:s5:s6 0x26c:rt:v0:s2 */
/* Address: 0x8013FBE4 */
/* Size: 796 bytes (199 instructions) */
/* register); (3) colorArg spill slot remap 72->92; (4) @SymbolSize: 796 — */
#include "types.h"

extern s32 prim_alloc();
extern void dtd_on_tpage(s32, s32, s32, s32, s32);
extern s32 GetClut(s32, s32);
extern void AddPrim();
extern void SetSemiTrans();
extern void SetSprt();

extern s32 activeBuff;
extern s16 screen_w[];
extern u8 ot_buff0[];
extern u8 etc_digit_uv_tbl[];
extern u8 etc_digit_uv_tbl_10_0;
extern u8 etc_digit_uv_tbl_10_1;

typedef struct {
    s32 unk0;
    u8  r;
    u8  g;
    u8  b;
    u8  pad7;
    s16 x;
    s16 y;
    s8  tile0;
    s8  tile1;
    s16 unkE;
    s16 w;
    s16 h;
} Obj;

void etc_number_disp(s32 a0, s32 a1, s32 a2, s32 a3)
{
    s32 negative;
    s32 value;
    s32 count;
    s32 xOffset;
    s32 colorArg;
    s32 digit;
    Obj *o;

    colorArg = a3 << 4;

    if (a0 < 0) {
        negative = 1;
        value = -a0;
    } else {
        negative = 0;
        value = a0;
    }

    count = 0;

    if (value >= 0) {
        xOffset = 0;
        do {
            o = (Obj *)prim_alloc(20, activeBuff);
            SetSprt(o);
            o->w = 8;
            o->h = 8;
            SetSemiTrans(o, 0);

            digit = value % 10;
            o->r = 128;
            o->g = 128;
            o->b = 128;
            o->x = screen_w[activeBuff + 10] + a1 - xOffset;
            o->y = screen_w[activeBuff + 12] + a2;
            o->tile0 = etc_digit_uv_tbl[digit * 2];
            o->tile1 = etc_digit_uv_tbl[digit * 2 + 1];
            o->unkE = (s16)GetClut(colorArg, 509);
            AddPrim(&ot_buff0[activeBuff * 1024 + 4], o);

            if (value < 10) {
                break;
            }
            value = value / 10;
            count++;
            xOffset += 8;
        } while (value >= 0);
    }

    if (negative != 0) {
        count++;
        xOffset = count << 3;

        o = (Obj *)prim_alloc(20, activeBuff);
        SetSprt(o);
        o->w = 8;
        o->h = 8;
        SetSemiTrans(o, 0);
        if (xOffset) {}

        o->r = 128;
        o->g = 128;
        o->b = 128;
        o->x = screen_w[activeBuff + 10] + a1 - xOffset;
        o->y = screen_w[activeBuff + 12] + a2;
        o->tile0 = etc_digit_uv_tbl_10_0;
        o->tile1 = etc_digit_uv_tbl_10_1;
        o->unkE = (s16)GetClut(colorArg, 509);
        AddPrim(&ot_buff0[activeBuff * 1024 + 4], o);
    }

    dtd_on_tpage(960, 0, 1, 0, 1);
}
