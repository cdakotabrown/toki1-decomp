/* @SymbolSize: 428 */
/* @PostProc: swap_instructions --offset1 0x3C --offset2 0x40 */
/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x44 --offset2 0x48 */
/* @PostProc: swap_instructions --offset1 0x48 --offset2 0x4C */
/* @PostProc: swap_instructions --offset1 0x50 --offset2 0x54 */
/* @PostProc: swap_instructions --offset1 0x54 --offset2 0x58 */
/* previous file combined `pad_trailing_nop --count 206` with @SymbolSize: */
#include "types.h"

extern u8 screen_w[];
extern u8 ot_buff0[];
extern u8 name_ent_kbd_num_uv_tbl[];
extern u32 activeBuff;

extern u8 *prim_alloc(s32, u32);
extern void SetSprt16(u8 *);
extern void SetSemiTrans(u8 *, s32);
extern s16 GetClut(s32, s32);
extern void AddPrim(u8 *, u8 *);
extern void dtd_on_tpage(s32, s32, s32, s32, s32);

s32 name_num_disp(u32 n, s32 x, s32 y)
{
    u8 *prim;
    s32 adv;

    adv = 0;
    for (;;) {
        prim = prim_alloc(16, activeBuff);
        SetSprt16(prim);
        SetSemiTrans(prim, 0);
        prim[4] = 128;
        prim[5] = 128;
        prim[6] = 128;
        *(s16 *)(prim + 8) =
            *(s16 *)(screen_w + activeBuff * 2 + 20) + x - adv;
        *(s16 *)(prim + 10) =
            *(s16 *)(screen_w + activeBuff * 2 + 24) + y;
        prim[12] = name_ent_kbd_num_uv_tbl[(n % 10) * 2];
        prim[13] = name_ent_kbd_num_uv_tbl[(n % 10) * 2 + 1];
        *(s16 *)(prim + 14) = GetClut(0, 496);
        AddPrim(ot_buff0 + (activeBuff << 10) + 12, prim);
        if (n < 10) {
            dtd_on_tpage(512, 0, 3, 0, 1);
            return 1;
        }
        adv += 8;
        n = n / 10;
    }
}
