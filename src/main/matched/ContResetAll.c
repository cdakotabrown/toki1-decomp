/* Address: 0x800936C8 */
/* Size: 184 bytes (46 instructions) */
/* Region: Library (>= 0x80086000) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: patch_stack_complete --old 32 --new 40 --shift-ra */
/* @PostProc: remap_stack_slots --map 20:28,28:36 */
/* @PostProc: patch_insn_reg_field --patch 0x58:rt:t5:t6 --patch 0x64:rt:v0:t3 --patch 0x6c:rt:v0:t3 --patch 0x6c:rd:t3:t4 --patch 0x74:rt:t3:t4 --patch 0x74:rd:t4:t5 --patch 0x78:rs:t4:t5 --patch 0x7c:rt:v0:t7 --patch 0x84:rt:v0:t7 --patch 0x84:rd:t6:t9 --patch 0x88:rs:t6:t9 --patch 0x88:rt:t5:t6 */
#include "types.h"

extern u32 _ss_score[];

extern void SsUtReverbOff(void);
extern s32 ReadDeltaValue(s16, s16);

void ContResetAll(s16 arg0, s16 arg1, u32 arg2)
{
    u8 *ptr;
    u8 idx;

    ptr = (u8 *)_ss_score[arg0] + (s32)arg1 * 168;
    SsUtReverbOff();

    idx = ptr[0x12];
    ptr[0x2C + idx] = idx;
    idx = ptr[0x12];
    ptr[0x13] = 0;
    ptr[0x14] = 0;
    *(u16 *)(ptr + 0x4C + idx * 2) = 127;
    idx = ptr[0x12];
    ptr[0x17 + idx] = 64;
    *(u32 *)(ptr + 0x88) = ReadDeltaValue(arg0, arg1);
}
