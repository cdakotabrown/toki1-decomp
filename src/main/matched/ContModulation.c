/* Address: 0x80093390, Size: 236 bytes (59 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @CompileEnv: IDO52_LDA_NOINREG_CLASSES=1 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: patch_stack_complete --old 112 --new 120 */
/* @PostProc: swap_registers --from s1 --to s2 --bidirectional */
/* @PostProc: remap_stack_slots --map '20:28,24:36,28:32,32:40,36:44,56:64,64:72,88:96,112:120,116:124' --keep-frame */
/* @PostProc: patch_insn_reg_field --patch 0x50:rt:t1:v0 --patch 0x58:rs:t1:s2 --patch 0x58:rt:s2:v0 --patch 0x6C:rt:t2:t1 --patch 0x74:rs:t2:t1 --patch 0xA8:rt:t3:t2 --patch 0xB0:rt:t3:t2 */
/* @PostProc: swap_instructions --offset1 0xD8 --offset2 0xDC */
/* 1. `@CompileEnv: IDO52_LDA_NOINREG_CLASSES=1` (Hook 3/4, sp-relative */
#include "types.h"

extern u8 *_ss_score[];

extern s32 SsUtGetProgAtr(s16, u8, u8 *);
extern s32 SsUtGetVagAtr(s16, u8, u32, u8 *);
extern s32 SsUtSetVagAtr(s16, u8, u32, u8 *);
extern s32 ReadDeltaValue(s16, s16);

void ContModulation(s16 channel, s16 voice, u8 tag)
{
    u8 cnt[24];
    u8 buf[32];
    u8 *rec;
    u8 *sub;
    u32 i;

    rec = _ss_score[channel] + voice * 168;
    sub = rec + rec[18];
    SsUtGetProgAtr(*(s16 *)(rec + 74), sub[44], cnt);

    for (i = 0; i < cnt[0]; i++) {
        SsUtGetVagAtr(*(s16 *)(rec + 74), sub[44], i, buf);
        buf[8] = tag;
        SsUtSetVagAtr(*(s16 *)(rec + 74), sub[44], i, buf);
    }

    *(s32 *)(rec + 136) = ReadDeltaValue(channel, voice);
}
