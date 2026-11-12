/* tt_curve_spawn - TT overlay, Address: 0x8014B078, Size: 272 bytes (68 insns) */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=-16 */
/* @PostProc: swap_instructions --offset1 0x70 --offset2 0x74 */
/* @PostProc: swap_instructions --offset1 0x74 --offset2 0x8c */
/* @PostProc: swap_instructions --offset1 0x78 --offset2 0x8c */
/* @PostProc: swap_instructions --offset1 0x7c --offset2 0x90 */
/* @PostProc: swap_instructions --offset1 0x80 --offset2 0x8c */
/* @PostProc: swap_instructions --offset1 0x84 --offset2 0x90 */
/* @PostProc: swap_instructions --offset1 0x88 --offset2 0x8c */
/* @PostProc: swap_instructions --offset1 0x8c --offset2 0x90 */
/* @PostProc: patch_insn_reg_field 0x7c:rd:t8:v1 0x94:rd:t9:t8 0x94:rt:t8:v1 0x98:rt:t9:t8 0x9c:rt:v0:t9 0xa0:rt:v1:t0 0xb8:rt:v0:t9 0xc0:rt:v1:t0 */
#include "types.h"

extern s32 tt_slot_get(void *a0, void *a1);
extern s32 tt_slot_kind_set(void *a0, u16 a1, s32 a2);

extern s32 tt_slot_buf;

void tt_curve_spawn(u8 param)
{
    u8 *p;
    u16 wide;
    s32 i;
    s32 v1;
    s32 arg2;
    s32 t9, t0;

    p = (u8 *)tt_slot_buf;
    wide = 64;
    i = 0;

    do {
        p = (u8 *)tt_slot_get(p, p + 7680);
        if (p == 0) {
            return;
        }
        v1 = -16 - (i << 4);
        if (param != 0) {
            arg2 = 4;
            *(s32 *)(p + 32) = 0x500000;
        } else {
            arg2 = 3;
            *(s32 *)(p + 32) = (s32)0xffb00000;
        }
        *(s32 *)(p + 36) = v1 << 16;
        t9 = *(s16 *)(p + 34);
        t0 = *(s16 *)(p + 38);
        *(s32 *)(p + 40) = 0;
        *(s32 *)(p + 44) = 0x20000;
        p[99] = param;
        *(s16 *)(p + 20) = t9;
        *(s16 *)(p + 22) = t0;
        tt_slot_kind_set(p, wide, arg2);
        i++;
        p += 120;
        *(s16 *)(p - 26) = 0;
        p[-24] = 0;
        *(u16 *)(p - 116) = wide;
        *(s16 *)(p - 110) = 0;
        *(s16 *)(p - 112) = 0;
        *(s16 *)(p - 114) = 0;
        p[-105] = 0;
        p[-106] = 0;
        p[-107] = 0;
    } while (i != 6);
}
