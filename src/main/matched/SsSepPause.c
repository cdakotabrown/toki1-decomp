/* Address: 0x800908E4 (recorded 0x800908FC -- a head symbol: both s16 */
/* Size: 244 bytes (61 words; the .sym's 252 includes two trailing */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @SymbolStart: 0x800908E4 */
/* @SymbolSize: 244 */
/* @CompileFlags: -Wc,-pic0 */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_FORCE_WEB_COLOR=18:a1 IDO52_AS1_TIE_TAKE=20,32,50 IDO52_UGEN_TEMP_REG=11:t5,16:t9 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: patch_stack_complete --old 40 --new 32 */
/* @PostProc: remap_stack_slots --map '28:24,44:36' */
#include "types.h"

typedef struct {
    u8  pad0[118];
    u8  f118[2];
    u8  f120[24];
    u32 flags144;
    u8  pad148[20];
} Chan168;

extern Chan168 *_ss_score[];
extern void SpuVmGetSeqVol(s16, u8 *, u8 *);

void SsSepPause(s16 channel, s16 voice)
{
    Chan168 **p = &_ss_score[channel];
    Chan168 *rec = &(*p)[voice];

    SpuVmGetSeqVol(channel, rec->f118, rec->f120);
    rec = &(*p)[(u32)voice];
    rec->flags144 &= ~1;
    rec = (Chan168 *)((u8 *)*p + ((voice * 21) << 3));
    rec->flags144 &= ~8;
    rec = (Chan168 *)((u8 *)*p + (((u32)voice * 21) << 3));
    rec->flags144 |= 2;
}
