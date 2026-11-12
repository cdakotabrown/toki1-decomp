/* @SymbolStart: 0x8013A8B8 */
/* @SymbolSize: 168 */
/* @PostProc: patch_stack_complete --old 40 --new 88 */
/* @PostProc: remap_stack_slots --from 32 --to 48 --from 34 --to 50 --from 36 --to 84 */
/* tt_cell3_hit - TT overlay, Address: 0x8013A8C0 */
#include "types.h"
#include "tokimeki/tt_box.h"

extern u8 *tt_boss_wk;
extern u8 tt_cell_box_tbl[];

extern void tt_cell_hit_scan(s16 *vec, u8 *arg, s32 mode);

void tt_cell3_hit(void)
{
    u8 *obj = tt_boss_wk;
    s16 vec[2];
    u32 idx = (u16)*(s16 *)(obj + 938);

    vec[0] = (s16)(*(s16 *)(obj + 20) + *(s16 *)(obj + 598));
    vec[1] = (s16)(*(s16 *)(obj + 22) + *(s16 *)(obj + 646));
    tt_cell_hit_scan(vec, tt_cell_box_tbl + idx * 6 + 102, -1);

    vec[0] = (s16)(*(s16 *)(obj + 20) + *(s16 *)(obj + 600));
    vec[1] = (s16)(*(s16 *)(obj + 22) + *(s16 *)(obj + 648));
    tt_cell_hit_scan(vec, ((u8 *)&((TT_BOX *)tt_cell_box_tbl)[23].x), 3);
}
