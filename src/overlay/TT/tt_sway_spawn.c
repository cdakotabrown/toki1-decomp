/* tt_sway_spawn - TT overlay, Address: 0x8014B494, Size: 256 bytes (64 insns) */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=-16 */
/* @PostProc: swap_instructions --offset1 0x3c --offset2 0x40 */
/* @PostProc: swap_instructions --offset1 0x70 --offset2 0x74 */
/* @PostProc: swap_instructions --offset1 0x74 --offset2 0x8c */
/* @PostProc: swap_instructions --offset1 0x78 --offset2 0x8c */
/* @PostProc: swap_instructions --offset1 0x7c --offset2 0x90 */
/* @PostProc: swap_instructions --offset1 0x80 --offset2 0x8c */
/* @PostProc: swap_instructions --offset1 0x84 --offset2 0x90 */
/* @PostProc: swap_instructions --offset1 0x88 --offset2 0x8c */
/* @PostProc: swap_instructions --offset1 0x8c --offset2 0x90 */
/* @PostProc: patch_insn_reg_field 0x7c:rd:t8:v1 0x94:rd:t9:t8 0x94:rt:t8:v1 0x98:rt:t9:t8 */
#include "types.h"

extern s32 tt_slot_get(void *a0, void *a1);
extern s32 tt_slot_kind_set(void *a0, u32 a1, s32 a2);

extern s32 tt_slot_buf;

void tt_sway_spawn(u32 param)
{
    u8 *rec;
    u8 *p2;
    u32 param8;
    s32 i;
    s32 v1;
    s32 arg2;
    s32 c50;
    s32 c1;
    s32 c64;
    s32 four;
    s32 cb0;

    rec = (u8 *)tt_slot_buf;
    param8 = param & 0xFF;
    param = 0;
    i = 0;
    c50 = 0x500000;
    c1 = 0x10000;
    c64 = 64;
    cb0 = (s32)0xFFB00000;

    do {
        rec = (u8 *)tt_slot_get(rec, rec + 7680);
        if (rec == 0) {
            return;
        }
        v1 = -16 - (i << 4);
        if (param8 != 0) {
            arg2 = 8;
            *(s32 *)(rec + 32) = c50;
        } else {
            arg2 = 7;
            *(s32 *)(rec + 32) = cb0;
        }
        *(s32 *)(rec + 36) = v1 << 16;
        *(s32 *)(rec + 40) = 0;
        *(s32 *)(rec + 44) = c1;
        rec[99] = (u8)param8;
        tt_slot_kind_set(rec, 67, arg2);
        i++;
        rec += 120;
        *(s16 *)(rec - 26) = 0;
        rec[-24] = 0;
        *(s16 *)(rec - 116) = (s16)c64;
        *(s16 *)(rec - 110) = 0;
        *(s16 *)(rec - 112) = 0;
        *(s16 *)(rec - 114) = 0;
        rec[-105] = 0;
        rec[-106] = 0;
        rec[-107] = 0;
    } while (i != 4);
}
