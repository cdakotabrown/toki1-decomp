/* tail-duplicates the epilogue.  The @Compiler: ido52-gas pipeline */
/* @SymbolSize: 272 */
/* @CompileFlags: -Wc,-pic0 -cckr */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_AS1_NO_JUMP_CHAIN=2 IDO52_AS1_TIE_REJECT=103,107,111 */
/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=12 IDO52_AS1_KEEP_SEXT=all IDO52_AS1_NO_SHIFT_COMBINE=all IDO52_AS1_XBB_SKIP_CAND=0 IDO52_AS1_NO_FALLTHRU_FILL=all */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: patch_insn_reg_field 0x38:rs:t2:a1 0x38:rt:a1:t2 */
#include "types.h"

extern s8  libsnd_cur_fake_program;
extern s8  libsnd_cur_tone_idx;
extern u8 *volatile _svm_tn;
extern u16 libsnd_note_pitch_tbl[];

s32 note2pitch2(u16 a0, u16 a1)
{
    s16 row;
    s16 hi;
    s16 lo;
    s16 sh;
    u32 val;

    row = (libsnd_cur_fake_program << 4) + libsnd_cur_tone_idx;
    hi = (a1 + (_svm_tn + 5)[row << 5]) >> 3;
    lo = 0;
    if (hi >= 16) {
        hi -= 16;
        lo = 1;
    }
    lo += (s16)(a0 - _svm_tn[(row * 32) + 4] + 60);
    val = libsnd_note_pitch_tbl[(s16)(lo % 12) * 16 + hi];
    sh = (s16)(lo / 12) - 5;
    if (sh > 0) {
        val = (u16)(val << sh);
    } else if (sh < 0) {
        val = (u16)(val >> -sh);
    }
    return val;
}
