/* @SymbolSize: 324 */
/* @PostProc: patch_insn_reg_field 0xcc:rt:t0:t1 0xd4:rd:t2:t0 0xd8:rs:t0:t1 0xd8:rt:t1:t2 0xdc:rs:t1:t0 */
/* tt_spread_move - TT overlay, Address: 0x8014BB20, Size: 324 bytes (81 insns) */
#include "types.h"

extern s32 rsin(s32 angle);
extern void tt_shot_aimed_make(s16 a0, s16 a1, s32 a2, s32 a3);

void tt_spread_move(u8 *obj)
{
    s32 c;
    u8 flag;

    if ((rsin(*(s32 *)(obj + 36) / 3072) << 9) < 0) {
        c = -(rsin(*(s32 *)(obj + 36) / 3072) << 9);
    } else {
        c = rsin(*(s32 *)(obj + 36) / 3072) << 9;
    }

    *(s32 *)(obj + 32) = (c + 0x100000) * (obj[80] - 2);

    if ((u32)*(u16 *)(obj + 94) >= 25U) {
        flag = obj[96];
        if (flag == 0) {
            *(s16 *)(obj + 94) = 0;
            obj[96] = flag + 1;
            tt_shot_aimed_make(*(s16 *)(obj + 20), *(s16 *)(obj + 22), 32, 160);
        }
    }
    if (obj[81] != 0) {
        *(s16 *)(obj + 94) = *(u16 *)(obj + 94) + 1;
    }
}
