/* @PostProc: patch_stack_complete --old 104 --new 120 */
/* @PostProc: remap_stack_slots --map '72:92' */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x40 */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x40 */
/* @PostProc: swap_instructions --offset1 0x1c --offset2 0x40 */
/* @PostProc: swap_instructions --offset1 0x20 --offset2 0x34 */
/* @PostProc: swap_instructions --offset1 0x24 --offset2 0x34 */
/* @PostProc: swap_instructions --offset1 0x28 --offset2 0x4c */
/* @PostProc: swap_instructions --offset1 0x2c --offset2 0x34 */
/* @PostProc: swap_instructions --offset1 0x30 --offset2 0x4c */
/* @PostProc: swap_instructions --offset1 0x38 --offset2 0x4c */
/* @PostProc: swap_instructions --offset1 0x3c --offset2 0x40 */
/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x4c */
/* @PostProc: swap_instructions --offset1 0x44 --offset2 0x4c */
/* @PostProc: swap_instructions --offset1 0x48 --offset2 0x4c */
/* @PostProc: patch_insn_reg_field 0x18:rt:v0:s0 0x50:rs:v0:s0 0x54:rd:s3:s4 0x80:rs:v0:s0 0xa4:rt:s4:s3 0x100:rt:s4:s3 0x120:rt:s3:s4 0x164:rs:s4:s3 0x188:rt:s4:s3 0x1a8:rt:s3:s4 0x1ec:rs:s4:s3 0x214:rt:s4:s3 0x230:rt:s3:s4 0x238:rt:s3:s4 0x284:rs:s4:s3 0x304:rt:s4:s3 0x308:rs:s3:s4 0x308:rt:s3:s4 0x30c:rs:s4:s3 */
/* @SymbolSize: 840 */
/* Address: 0x80148EF8 */
/* Size: 836 bytes (209 instructions) + 1 alignment nop (.sym window 840) */
/* with `@PostProc: pad_trailing_nop --count 210` faking the whole body. */
#include "types.h"

extern s32 prim_alloc();
extern void dtd_on_tpage();
extern s32 GetClut();
extern void AddPrim();
extern void SetSemiTrans();
extern void SetSprt();

extern s16 screen_w[];
extern u8 ot_buff0[];
extern s32 activeBuff;
extern u8 etc_num_u_tbl[];

void etc_num_disp(s32 num, s32 ox, s32 oy, s32 kind, volatile s32 slot)
{
    u8 *o;
    u32 n;
    u32 ten;
    s32 col;
    s32 k;
    s32 sl;

    sl = slot;
    k = kind;
    col = 0;
    if (sl == 1) {
        if ((u32)num < 10) {
            k = 0;
        } else if ((u32)num < 100) {
            k = 1;
        } else {
            k = 2;
        }
    }

    n = num;
    ten = 10;
    for (;;) {
        o = (u8 *)prim_alloc(20, activeBuff);
        SetSprt(o);
        SetSemiTrans(o, 0);
        *(u8 *)(o + 4) = 128;
        *(u8 *)(o + 5) = 128;
        *(u8 *)(o + 6) = 128;
        switch (k) {
        case 0:
            *(s16 *)(o + 8) =
                screen_w[activeBuff + 10] + ox - col * 8 - slot * 2;
            *(s16 *)(o + 10) = screen_w[activeBuff + 12] + oy;
            *(s16 *)(o + 16) = 8;
            *(s16 *)(o + 18) = 12;
            *(u8 *)(o + 12) = etc_num_u_tbl[n % ten];
            *(u8 *)(o + 13) = 0;
            break;
        case 1:
            *(s16 *)(o + 8) =
                screen_w[activeBuff + 10] + ox - col * 8 + slot * 4;
            *(s16 *)(o + 10) = screen_w[activeBuff + 12] + oy;
            *(s16 *)(o + 16) = 8;
            *(s16 *)(o + 18) = 12;
            *(u8 *)(o + 12) = etc_num_u_tbl[n % ten];
            *(u8 *)(o + 13) = 16;
            break;
        case 2:
            *(s16 *)(o + 8) =
                screen_w[activeBuff + 10] + ox - col * 6 + slot * 4;
            *(s16 *)(o + 10) = screen_w[activeBuff + 12] + oy;
            *(s16 *)(o + 16) = 6;
            *(s16 *)(o + 18) = 12;
            *(u8 *)(o + 12) = etc_num_u_tbl[n % ten];
            *(u8 *)(o + 13) = 32;
            break;
        }
        *(s16 *)(o + 14) = GetClut(256, sl + 496);
        AddPrim(&ot_buff0[activeBuff * 1024 + 4], o);
        if (n < 10U) {
            dtd_on_tpage(320, 0, 1, 0, 0);
            return;
        }
        col = col + 1;
        n = n / ten;
    }
}
