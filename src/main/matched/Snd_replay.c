/* Address: 0x80091B20 */
/* Size: 128 bytes (29 instructions + 3 trailing alignment nops) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @CompileFlags: -Wc,-pic0 */
/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=168 IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_UGEN_TEMP_REG=3:t6 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: patch_insn_reg_field --patch 0x24:rd:v0:v1 --patch 0x2C:rs:v0:v1 --patch 0x3C:rt:t6:t0 --patch 0x44:rd:t0:v0 --patch 0x48:rt:t6:t0 --patch 0x48:rs:t0:v0 --patch 0x50:rs:v0:v1 --patch 0x5C:rd:v1:a2 --patch 0x60:rs:v1:a2 --patch 0x70:rs:v1:a2 */
#include "types.h"

extern u8 *_ss_score[];

void Snd_replay(s16 channel, s16 voice)
{
    _ss_score[channel][voice * 168 + 43] = 1;
    *(s32 *)(_ss_score[channel] + voice * 21 * 8 + 144) &= ~8;
}
