/* Function: _spu_interruptDMA */
/* Address: 0x80095D00 */
/* Size: 144 bytes (36 instructions) */
/* Region: Library (0x80086000+), compiler IDO 5.2 (no `j`, t6/t7/t8/t9 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: patch_insn_reg_field --patch 0x64:rt:t9:t8 --patch 0x68:rs:t9:t8 --patch 0x68:rt:t9:t8 --patch 0x70:rs:t9:t8 */
/* @PostProc: strip_leading_instructions --count auto */
/* @CompileEnv: IDO52_AS1_KEEP_ANDI=occ:0,2 */
/* @CompileEnv: IDO52_AS1_NO_LA_PULL=all */
/* @CompileEnv: IDO52_AS1_NO_FALLTHRU_FILL=all */
/* @CompileEnv: IDO52_AS1_NO_BOPT=occ:0 */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=2:3 */
/* @CompileEnv: IDO52_UGEN_TEMP_REG_INPLACE=2 */
#include "types.h"

extern s32 spu_dma_mode;
extern u8 *_spu_RXX;
extern void (*volatile _spu_transferCallback)(void);

extern void _waitfs(void);
extern void DeliverEvent(s32 arg0, s32 arg1);

void _spu_interruptDMA(void)
{
    u8 *b;
    s32 t;

    if (spu_dma_mode == 0) {
        _waitfs();
        _waitfs();
    }

    b = _spu_RXX;
    t = *(u16 *)(b + 426);
    t = t & 0xFFCF;
    t = t & 0xFFFF;
    *(u16 *)(b + 426) = (u16)t;

    if (_spu_transferCallback != 0) {
        _spu_transferCallback();
    } else {
        DeliverEvent(0xF0000009, 32);
    }
}
