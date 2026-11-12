/* tt_rank_insert - TT overlay, Address: 0x80145E50, Size: 416 bytes (104 insns) */
/* @SymbolSize: 416 */
/* @PostProc: patch_stack_complete --old 48 --new 64 */
/* @PostProc: remap_stack_slots --map '36:48' */
/* @PostProc: patch_insn_reg_field 0x18c:rs:t5:s0 0x18c:rt:s0:t5 */
#include "types.h"

extern u8 *tt_game_wk;
extern u8 *tt_scroll_wk;

extern void memcpy(u8 *dst, u8 *src, s32 n);
extern void tt_name_set(u8 *rec, s32 id);

s32 tt_rank_insert(void)
{
    u8 *r;
    u8 *c;

    r = tt_game_wk;
    c = tt_scroll_wk;
    if (*(u16 *)(c + 8) < (u32)*(u16 *)(r + 48)) {
        *(u16 *)(r + 60) = *(u16 *)(r + 54);
        *(u16 *)(r + 54) = *(u16 *)(r + 48);
        *(u16 *)(r + 48) = *(u16 *)(c + 8);
        memcpy(r + 62, r + 56, 3);
        memcpy(r + 56, r + 50, 3);
        *(s16 *)(c + 24) = 0;
    } else if (*(u16 *)(c + 8) < (u32)*(u16 *)(tt_game_wk + 54)) {
        *(u16 *)(r + 60) = *(u16 *)(r + 54);
        *(u16 *)(r + 54) = *(u16 *)(c + 8);
        memcpy(r + 62, r + 56, 3);
        *(s16 *)(c + 24) = 1;
    } else if (*(u16 *)(c + 8) < (u32)*(u16 *)(tt_game_wk + 60)) {
        *(u16 *)(r + 60) = *(u16 *)(c + 8);
        *(s16 *)(c + 24) = 2;
    } else {
        return 0;
    }
    r[*(u16 *)(c + 24) * 6 + 50] = 10;
    r[*(u16 *)(c + 24) * 6 + 51] = 47;
    r[*(u16 *)(c + 24) * 6 + 52] = 47;
    r[*(u16 *)(c + 24) * 6 + 53] = 255;
    tt_name_set(r + *(u16 *)(c + 24) * 6 + 50,
                  (s32)*(u16 *)(c + 24) + 43);
    return 1;
}
