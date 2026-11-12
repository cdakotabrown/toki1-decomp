/* Function: bustup_mouse_on */
/* Address: 0x80093390 */
/* Size: 236 bytes (59 instructions) */
/* Region: Library (0x80086000+) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: custom_script custom_bustup_mouse_on.py */
#include "types.h"

extern u32 _ss_score[];
extern void SsUtGetProgAtr(s16 a0, u8 a1, u8 *a2);
extern void SsUtGetVagAtr(s16 a0, u8 a1, s32 a2, u8 *a3);
extern void SsUtSetVagAtr(s16 a0, u8 a1, s32 a2, u8 *a3);
extern s32 ReadDeltaValue(s16 a0, s16 a1);

void bustup_mouse_on(s16 a0, s16 a1, u8 a2) {
    u8 *base;
    u8 *sprite;
    s32 i;
    u8 result[24];
    u8 data[32];

    base = (u8 *)_ss_score[a0] + a1 * 168;
    sprite = base + base[18];

    SsUtGetProgAtr(*(s16 *)(base + 74), sprite[44], result);

    i = 0;
    if (result[0] == 0) {
        goto end;
    }

    do {
        SsUtGetVagAtr(*(s16 *)(base + 74), sprite[44], i, data);
        data[8] = a2;
        SsUtSetVagAtr(*(s16 *)(base + 74), sprite[44], i, data);
        i++;
    } while ((u32)i < result[0]);

end:
    *(s32 *)(base + 136) = ReadDeltaValue(a0, a1);
}
