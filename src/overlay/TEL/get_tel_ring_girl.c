/* @SymbolStart: 0x8013BD0C */
/* @SymbolSize: 92 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: patch_insn_reg_field 0x2c:rd:t7:t8 0x30:rs:t7:t8 0x38:rt:t8:t9 0x40:rs:t8:t9 */
/* @PostProc: patch_stack_complete --old 32 --new 24 */
/* get_tel_ring_girl - TEL overlay, Address: 0x8013BD10, Size: 88 bytes (22 insns) */













#include "types.h"

extern u8 screen_w[];

s32 get_tel_ring_girl(void)
{
    s32 i;
    u8 *base;
    s32 flags;
    s32 slot;

#line 100
    i = 0;
#line 99
    base = screen_w;
#line 101
    do {
        if ((*(s32 *)(base + 456) << 30) < 0) {
            if (*(s16 *)(base + 454) >= 91) {
                slot = i;
                goto done;
            }
        }
        i++;
        base += 56;
    } while (i != 11);

done:
    return slot;
}
