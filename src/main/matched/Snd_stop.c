/* Function: Snd_stop */
/* Address: 0x800902D0   (true entry 0x800902A0 -- see WINDOW below) */
/* Size: 304 bytes (76 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* 1. ⭐ THE MISSING `@PostProc: strip_leading_instructions`.  The file had */
/* @SymbolStart: 0x800902A0 */
/* @SymbolSize: 304 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_FORCE_WEB_COLOR=10:t0,12:v1,38:v1,52:t3 IDO52_UGEN_TEMP_REG=3:t9,4:t4,5:t5,11:t6 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: patch_insn_reg_field 0x10:rd:t9:t8 0x14:rs:t9:t8 0x14:rd:t4:t8 0x24:rt:t4:t8 0x24:rd:t5:t8 0x2c:rs:t5:t8 0x34:rd:t6:t8 0x40:rt:t6:t8 0x94:rt:a0:t1 0xa0:rt:v1:t2 0xa8:rt:a0:t1 0xac:rt:v1:t2 */
#include "types.h"

extern volatile u8 *_ss_score[];
extern void SpuVmSeqKeyOff(s32 arg0);

void Snd_stop(s16 arg0, s16 arg1) {
    s32 offset;
    volatile u8 *t0;
    s32 i;
    volatile u8 *v0;
    volatile u8 *a2;
    s32 v1val;
    s32 t1val, t2val;
    s16 t3val;

    offset = ((arg1 << 2) + arg1);
    offset = (offset << 2) + arg1;
    offset = offset << 3;
    t0 = _ss_score[arg0] + offset;

    SpuVmSeqKeyOff((arg1 << 8) | arg0);

    i = 0;

    t0[43] = 0;
    *(volatile s32 *)(t0 + 128) = 0;
    t0[19] = 0;
    t0[20] = 0;
    t0[41] = 0;
    t0[21] = 0;
    t0[22] = 0;
    t0[42] = 0;
    t0[18] = 0;
    *(volatile s16 *)(t0 + 70) = 0;
    t0[39] = 0;
    t0[40] = 0;
    t0[16] = 0;
    t0[17] = 0;

    v1val = *(volatile s32 *)(t0 + 8);
    t1val = *(volatile s32 *)(t0 + 124);
    *(volatile s32 *)(t0 + 4) = v1val;
    *(volatile s32 *)(t0 + 12) = v1val;
    t2val = *(volatile s32 *)(t0 + 132);
    t3val = *(volatile s16 *)(t0 + 112);
    *(volatile s32 *)(t0 + 136) = t1val;
    *(volatile s32 *)(t0 + 140) = t2val;

    v0 = t0;
    a2 = t0;
    *(s16 *)(t0 + 110) = t3val;

    do {
        v0[44] = (u8)i;
        v0[23] = 64;
        *(s16 *)(a2 + 76) = 127;
        v0[24] = 64;
        v0[45] = (u8)(i + 1);
        *(s16 *)(a2 + 78) = 127;
        v0[46] = (u8)(i + 2);
        v0[25] = 64;
        *(s16 *)(a2 + 80) = 127;
        v0[47] = (u8)(i + 3);
        i += 4;
        v0[26] = 64;
        *(s16 *)(a2 + 82) = 127;
        v0 += 4;
        a2 += 8;
    } while (i != 16);

    *(u16 *)(t0 + 118) = 127;
    *(u16 *)(t0 + 120) = 127;
}
