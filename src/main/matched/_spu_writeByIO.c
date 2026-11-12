/* Function: _spu_writeByIO */
/* Address: 0x80095AF4 */
/* Size: 484 bytes (121 instructions) */
/* Region: Library (0x80086000-0x800BFFFF) */
/* Compiler: IDO 5.2 (t6..t9 temporaries, `b` not `j`, ASCENDING s-register */
/* wrong: `@CompileFlags: -Wo,-do_opt_saved_regs` reproduces retail's */
/* @CompileFlags: -Wo,-do_opt_saved_regs */
/* @CompileEnv: IDO52_AS1_KEEP_ANDI=all IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_FORCE_WEB_COLOR=5:s7,70:s1,25:s1 IDO52_AS1_XBB_SKIP_CAND=all IDO52_AS1_NO_FALLTHRU_FILL=2 IDO52_AS1_FTF_BACKWARD=occ:2 IDO52_COPYPROP_VAR=-24:4 IDO52_COPYPROP_SITE=lod:1:deny,lod:3:deny IDO52_AS1_TIE_TAKE=63,66,96 IDO52_AS1_TIE_REJECT=50 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: remap_stack_slots --map 48:52,52:56,56:60 */
/* @PostProc: swap_instructions --offset1 0x1c --offset2 0x20 */
/* @PostProc: swap_instructions --offset1 0x34 --offset2 0x38 */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x68 */
/* @PostProc: swap_instructions --offset1 0x194 --offset2 0x1a0 */
#include "types.h"

typedef struct {
    char pad[422];
    volatile u16 reg1a6;
    volatile u16 reg1a8;
    volatile u16 reg1aa;
    u16 pad3;
    volatile u16 reg1ae;
} CDRegs;

extern CDRegs *_spu_RXX;
extern u16 spu_tsa;
extern s32 tcount;
extern u8 libspu_writebyio_wrdy_timeout_fmt[];
extern u8 libspu_writebyio_wrdy_wait_str[];
extern u8 libspu_writebyio_dmaf_timeout_fmt[];
extern u8 libspu_writebyio_dmaf_wait_str[];

extern void _waitfs(void);
extern void printf(void *arg0, void *arg1);

void _spu_writeByIO(u16 *arg0, u32 arg1)
{
    CDRegs *hw;
    u32 saved;
    u16 v;
    u32 chunk;
    s32 loop_cnt;
    s32 n2;
    s32 i;

    hw = _spu_RXX;
    saved = hw->reg1ae;
    saved = (u16)(saved & 0x7FF);

    hw->reg1a6 = spu_tsa;
    _waitfs();

    if (arg1 == 0) {
        goto finish;
    }

    do {
        chunk = (arg1 >= 65) ? 64 : arg1;
        loop_cnt = 0;
        i = 0;

        if ((s32)chunk > 0) {
            do {
                v = *arg0++;
                hw->reg1a8 = v;
                i += 2;
            } while (i < (s32)chunk);
        }

        v = hw->reg1aa;
        v = v & 0xFFCF;
        v = v | 0x10;
        hw->reg1aa = v;
        _waitfs();

        if (hw->reg1ae & 0x400) {
            do {
                loop_cnt++;
                if (loop_cnt >= 5001) {
                    tcount = loop_cnt;
                    printf(libspu_writebyio_wrdy_timeout_fmt, libspu_writebyio_wrdy_wait_str);
                    hw = _spu_RXX;
                    break;
                }
            } while (hw->reg1ae & 0x400);
        }

        _waitfs();
        _waitfs();
        arg1 -= chunk;
    } while (arg1 != 0);

finish:
    n2 = 0;
    v = hw->reg1aa;
    v = v & 0xFFCF;
    hw->reg1aa = v;

    if (saved == (hw->reg1ae & 0x7FF)) {
        tcount = n2;
        return;
    }
    do {
        n2++;
        if (n2 >= 5001) {
            tcount = n2;
            printf(libspu_writebyio_dmaf_timeout_fmt, libspu_writebyio_dmaf_wait_str);
            return;
        }
    } while (saved != (hw->reg1ae & 0x7FF));

    tcount = n2;
}
