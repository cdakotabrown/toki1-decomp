/* Address: 0x80092E38 (the recorded symbol 0x80092E68 is the PROLOGUE) */
/* Size: 224 bytes (56 instructions) */
/* Region: Library (0x80086000+), IDO 5.2, stock pic0 */
/* @SymbolStart: 0x80092E38 */
/* @SymbolSize: 224 */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_AS1_XBB_BUDGET=0 IDO52_UGEN_TEMP_REG=8:t8,9:t9,10:t6,11:t7 IDO52_FORCE_WEB_COLOR=0:v1,14:v0,4:t1,10:t2,23:t4,35:t0,37:t3,42:t5 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: patch_insn_reg_field --patch 0x58:rs:t7:a3 --patch 0x58:rt:a3:t7 --patch 0x78:rd:t8:t4 --patch 0x7C:rt:t8:t4 */
#include "types.h"

extern u8 *_ss_score[];

extern void SpuVmKeyOn(u32 id, s32 a1, s32 a2, s32 a3, u32 a4, u32 a5);
extern void SpuVmKeyOff(u32 id, s32 a1, s32 a2, s32 a3);

void NoteOn(s32 channel, s32 voice, u8 tag, s32 vol)
{
    u8 *rec;
    u8 *sub;
    s32 idx;
    s32 scaled;
    s32 pan;
    u8 tagv;

    rec = _ss_score[(s16)channel] + (s16)voice * 168;
    idx = rec[18];
    vol = vol & 0xFF;
    scaled = ((u32)vol * (u32)*(s16 *)(rec + 76 + idx * 2)) / 127;
    sub = rec + idx;
    pan = sub[23];
    tagv = tag;
    tag = 0;
    scaled = (s16)scaled;
    if (vol != 0) {
        SpuVmKeyOn(((s16)voice << 8) | (s16)channel, *(s16 *)(rec + 74),
                      sub[44], tagv, (u16)scaled, (u16)pan);
    } else {
        SpuVmKeyOff(((s16)voice << 8) | (s16)channel, *(s16 *)(rec + 74),
                      sub[44], tagv);
    }
}
