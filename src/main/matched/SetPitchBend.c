/* Address: 0x80094594 (recorded symbol 0x800945C0 is the prologue) */
/* Size: 180 bytes (45 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @SymbolStart: 0x80094594 */
/* @SymbolSize: 180 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: patch_stack_complete --old 32 --new 40 */
/* @PostProc: remap_stack_slots --map '32:40,36:44,40:48' --keep-frame */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=0:t3,5:t2,22:t1 IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_AS1_NO_COPY_PROP=2,3,4 IDO52_AS1_KEEP_DEAD_RELOAD=0 IDO52_AS1_TIE_TAKE=80,84 */
#include "types.h"

extern u8 *_ss_score[];
extern void SpuVmPitchBend(s32, s16, u8, u8);
extern s32 ReadDeltaValue(s32, s32);

void SetPitchBend(s16 ch, s16 vo, u32 arg2)
{
    u8 *rec;
    u8 *p;
    u8 *sub;
    u32 k;
    u8 c;

    vo = (vo << 16) >> 16;
    ch = (ch << 16) >> 16;
    rec = _ss_score[ch] + vo * 168;
    p = *(u8 **)(rec + 4);
    c = *p;
    *(u8 **)(rec + 4) = p + 1;
    k = rec[18];
    sub = rec + k;
    SpuVmPitchBend((vo << 8) | ch, *(s16 *)(rec + 74), sub[44], c);
    *(s32 *)(rec + 136) = ReadDeltaValue(ch, vo);
}
