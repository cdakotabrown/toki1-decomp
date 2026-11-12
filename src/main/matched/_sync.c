/* Address: 0x800A26AC, Size: 468 bytes (117 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* 52/117 Fixup 5 with the old @Compiler: ido52-gas recipe. */
/* such thing, so `@Compiler: ido52-gas` gets the and-form everywhere at */
/* @CompileFlags: -Wc,-pic0 -Wo,-do_opt_saved_regs */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=42:s1,43:s0 IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_AS1_NO_BRANCH_FOLD=all IDO52_AS1_NO_FALLTHRU_FILL=1 IDO52_AS1_NO_LA_PULL=1 IDO52_AS1_NO_PASS1_PROP=all IDO52_AS1_NO_JUMP_CHAIN=17 IDO52_CONST_DEF_BB=43:13:- IDO52_CONST_DEF_EDGE=43:10:j IDO52_AS1_TIE_REJECT=4,8,19 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: patch_insn_reg_field 0x138:rd:v0:a0 0x13c:rs:v0:a0 */
#include "types.h"

extern s32 _qin;
extern s32 _qout;
extern volatile u32 *libgpu_dma2_chcr_ptr;
extern volatile u32 *GPU_status;

extern void set_alarm(void);
extern void _exeque(void);
extern s32 get_alarm(void);

s32 _sync(s32 mode)
{
    s32 n;

    if (mode == 0) {
        set_alarm();
        while (_qin != _qout) {
            _exeque();
            if (get_alarm() != 0) {
                return -1;
            }
        }
        while ((*libgpu_dma2_chcr_ptr & 0x01000000) || !(*GPU_status & 0x04000000)) {
            if (get_alarm() != 0) {
                return -1;
            }
        }
        return 0;
    }

    n = (_qin - _qout) & 0x3f;
    if (n != 0) {
        _exeque();
    }
    if ((*libgpu_dma2_chcr_ptr & 0x01000000) || !(*GPU_status & 0x04000000)) {
        return (n != 0) ? n : 1;
    }
    return n;
}
