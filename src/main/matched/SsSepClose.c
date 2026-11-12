/* Address: 0x80090610 */
/* Size: 576 bytes (144 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 -O2 */
/* @CompileFlags: -Wc,-pic0 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=8:v1 IDO52_CONST_NOINREG_VALUES=168 IDO52_NO_CSE_OPCODES=mpy */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: remap_stack_slots --keep-frame --map 20:28,24:32 */
/* @PostProc: patch_stack_complete --old 24 --new 32 */
#include "types.h"

extern s32 _snd_openflag;
extern s16 _snd_seq_t_max;
extern u8 *_ss_score[];

extern void SpuVmSetSeqVol(s16, s32, s32, s32);
extern void SpuVmSeqKeyOff(s16);

void SsSepClose(s16 arg0)
{
    s32 i;
    u8 **pp;

    SpuVmSetSeqVol(arg0, 0, 0, 1);
    SpuVmSeqKeyOff(arg0);

    _snd_openflag &= ~(1 << arg0);

    i = 0;
    if (_snd_seq_t_max > 0) {
        pp = &_ss_score[arg0];
        do {
            *(s32 *)(*pp + i * 168 + 144) = 0;
            goto L1; L1:
            *(s8 *)(*pp + i * 168) = -1;
            goto L2; L2:
            *(s16 *)(*pp + i * 168 + 60) = 0;
            goto L3; L3:
            *(s16 *)(*pp + i * 168 + 62) = 0;
            goto L4; L4:
            *(s32 *)(*pp + i * 168 + 148) = 0;
            goto L5; L5:
            *(s32 *)(*pp + i * 168 + 152) = 0;
            goto L6; L6:
            *(s16 *)(*pp + i * 168 + 64) = 0;
            goto L7; L7:
            *(s32 *)(*pp + i * 168 + 164) = 0;
            goto L8; L8:
            *(s32 *)(*pp + i * 168 + 160) = 0;
            goto L9; L9:
            *(s32 *)(*pp + i * 168 + 156) = 0;
            goto L10; L10:
            *(s16 *)(*pp + i * 168 + 66) = 0;
            goto L11; L11:
            *(s16 *)(*pp + i * 168 + 114) = 127;
            goto L12; L12:
            *(s16 *)(*pp + i * 168 + 116) = 127;
            i = (i | 0) + 1;
        } while (i < _snd_seq_t_max);
    }
}
