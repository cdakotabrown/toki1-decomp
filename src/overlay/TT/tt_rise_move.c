/* @PostProc: patch_insn_reg_field 0x4c:rt:a0:v1 0x54:rd:v1:a0 0x54:rt:a0:v1 0x58:rt:v1:a0 0x64:rs:a0:v1 0x7c:rt:v1:a0 */
/* tt_rise_move - TT overlay, Address: 0x8014BF44, Size: 348 bytes (87 insns) */
#include "types.h"

extern s32 rcos(s32 angle);
extern void tt_shot_aimed_make(s16 a0, s16 a1, s32 a2, s32 a3);

void tt_rise_move(u8 *obj)
{
    s32 v;
    s32 a;
    s32 v1;
    s32 c;

    switch (*(u16 *)(obj + 6)) {
    case 0:
        v = obj[80] - 2;
        a = *(s32 *)(obj + 36) >> 16;
        *(s32 *)(obj + 32) = (v << 21) + (v * a * a) * 48;
        if (*(s32 *)(obj + 36) < 0x540000) {
            *(s32 *)(obj + 44) = 0;
            *(s16 *)(obj + 6) = 1;
            *(s16 *)(obj + 110) = 1024;
        }
        obj[60] = 1;
        return;
    case 1:
        *(s32 *)(obj + 36) = (rcos(*(u16 *)(obj + 110)) << 9) + 0x500000;
        v1 = *(u16 *)(obj + 110);
        if ((u32)v1 >= 3201U) {
            *(s32 *)(obj + 44) = 0x20000;
            *(s16 *)(obj + 110) = 0;
            *(s16 *)(obj + 6) = 2;
            v1 = *(u16 *)(obj + 110);
        }
        *(s16 *)(obj + 110) = v1 + 48;
        obj[60] = 0;
        return;
    case 2:
        if ((u32)*(u16 *)(obj + 94) >= 5U) {
            c = obj[96];
            if (c == 0) {
                *(s16 *)(obj + 94) = 0;
                obj[96] = c + 1;
                tt_shot_aimed_make(*(s16 *)(obj + 20), *(s16 *)(obj + 22), 32,
                              160);
            }
        }
        if (obj[81] != 0) {
            *(s16 *)(obj + 94) = *(u16 *)(obj + 94) + 1;
        }
        return;
    default:
        return;
    }
}
