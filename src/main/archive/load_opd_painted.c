/* Function: load_opd */
/* Address: 0x80093568 */
/* Size: 352 bytes (88 instructions) */
/* Region: Library (0x80086000+) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: patch_stack_complete --old 104 --new 120 --shift-ra */
/* @PostProc: custom_script func_80093568_transform.py */
#include "types.h"

extern u8 *_ss_score[];
extern void SsUtGetProgAtr(s16 pitch, u8 prog, u8 *count_out);
extern void SsUtGetVagAtr(s16 pitch, u8 prog, s32 voice_idx, u8 *data);
extern void SsUtSetVagAtr(s16 pitch, u8 prog, s32 voice_idx, u8 *data);
extern s32 ReadDeltaValue(s16, s16);

void load_opd(s16 arg0, s16 arg1, u8 value) {
    u8 voice_data[32];
    u8 voice_count;
    u8 *base;
    u8 *sub;
    u8 idx;
    u32 i;

    base = _ss_score[arg0] + (arg1 * 21 * 8);
    idx = base[18];
    sub = base + idx;

    SsUtGetProgAtr(*(s16 *)(base + 74), sub[44], &voice_count);

    if (value < 64) {
        i = 0;
        if (voice_count != 0) {
            do {
                SsUtGetVagAtr(*(s16 *)(base + 74), sub[44], i, voice_data);
                voice_data[1] = 2;
                SsUtSetVagAtr(*(s16 *)(base + 74), sub[44], i, voice_data);
                i++;
            } while (i < voice_count);
        }
    } else if (value < 128) {
        i = 0;
        if (voice_count != 0) {
            do {
                SsUtGetVagAtr(*(s16 *)(base + 74), sub[44], i, voice_data);
                voice_data[1] = 0;
                SsUtSetVagAtr(*(s16 *)(base + 74), sub[44], i, voice_data);
                i++;
            } while (i < voice_count);
        }
    }

    *(s32 *)(base + 136) = ReadDeltaValue(arg0, arg1);
}
