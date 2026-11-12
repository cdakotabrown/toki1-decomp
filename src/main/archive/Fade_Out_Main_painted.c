/* Function: Fade_Out_Main */
/* Address: 0x800945C0 */
/* Size: 136 bytes (34 instructions) */
/* Region: Library (0x80086000+) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack_complete --old 48 --new 40 */
/* @PostProc: custom_script custom_func_800945C0.py */
#include "types.h"

extern void SpuVmPitchBend(s32 a0, s16 a1, u8 a2, u8 a3);
extern s32 ReadDeltaValue(s16 a0, s16 a1);

void Fade_Out_Main(s16 track, s16 channel, u8 *base) {
    u8 *entry;
    u8 *ptr;
    u8 byte;
    u8 idx;
    s32 result;

    entry = base + ((track + channel) << 3);

    ptr = *(u8 **)(entry + 4);
    idx = *(u8 *)(entry + 18);
    byte = *ptr;
    *(u8 **)(entry + 4) = ptr + 1;

    SpuVmPitchBend((channel << 8) | track, *(s16 *)(entry + 74),
                  *(u8 *)(entry + idx + 44), byte);

    result = ReadDeltaValue(track, channel);
    *(s32 *)(entry + 136) = result;
}
