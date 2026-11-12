/* Function: ContRpn2 */
/* Address: 0x80093A40 */
/* Size: 72 bytes (18 instructions) */
/* Region: Library (0x80086000+) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: custom_script custom_event_face_kuchi0.py */
#include "types.h"

extern s32 ReadDeltaValue(s32 channel, s32 voice);

void ContRpn2(s32 channel, s32 voice, s32 param) {
    u8 *v1;

    v1 = (u8 *)channel + (voice * 8);
    v1[20] = (u8)(param & 0xFF);
    v1[41] = (u8)v1[41] + 1;
    *(u32 *)(v1 + 136) = ReadDeltaValue(channel, voice);
}
