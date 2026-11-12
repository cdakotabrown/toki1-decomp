/* Address: 0x800939C8 */
/* Size: 72 bytes (18 instructions)   [recorded values; see CORRECTED WINDOW] */
/* Region: Library (0x80086000+) */
/* Debug Symbol: ContRpn1 (from o.bin, exact match) */
/* Compiler: IDO 5.2 (epilogue restores ra then s-regs ascending, scratch */
/* @SymbolStart: 0x80093998 */
/* @SymbolSize: 120 */
/* @CompileEnv: IDO52_AS1_KEEP_ANDI=all */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: patch_insn_reg_field --patch 0x0:rd:t8:a1 --patch 0x4:rt:t8:a1 --patch 0x8:rd:t6:a0 --patch 0xc:rt:t6:a0 --patch 0x10:rd:t3:t8 --patch 0x14:rs:t3:t8 --patch 0x14:rd:t3:t8 --patch 0x18:rd:t1:t6 --patch 0x1c:rt:t2:t7 --patch 0x20:rs:t2:t7 --patch 0x20:rt:t1:t6 --patch 0x20:rd:t2:t7 --patch 0x24:rt:t3:t8 --patch 0x24:rd:t3:t8 --patch 0x28:rs:t2:t7 --patch 0x28:rt:t2:t7 --patch 0x2c:rs:t3:t8 --patch 0x2c:rd:t3:t8 --patch 0x34:rt:t3:t8 --patch 0x34:rd:t3:t8 --patch 0x3c:rs:t2:t7 --patch 0x3c:rt:t3:t8 --patch 0x40:rt:t4:t9 --patch 0x44:rt:t0:a2 --patch 0x48:rs:t4:t9 --patch 0x48:rt:t5:t0 --patch 0x4c:rt:t0:a2 --patch 0x50:rt:t5:t0 */
#include "types.h"

extern u8 *_ss_score[];
extern s32 ReadDeltaValue(s32, s32);

void ContRpn1(s16 channel, s16 voice, s32 param) {
    u8 *v1;

    param = param & 0xFF;
    v1 = _ss_score[channel] + voice * 168;
    v1[19] = param;
    v1[41] = v1[41] + 1;
    *(s32 *)(v1 + 136) = ReadDeltaValue(channel, voice);
}
