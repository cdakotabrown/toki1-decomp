/* Address: 0x80097A34 */
/* Size: 108 bytes (27 instructions) */
/* Region: IDO Library (0x80086000+) */
/* @CompileEnv: IDO52_AS1_XBB_SKIP_CAND=0 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: patch_insn_reg_field 0x34:rt:t0:t8 0x38:rt:t8:v1 0x3c:rt:t8:v1 0x3c:rd:t9:v1 0x40:rs:t9:v1 0x44:rt:t0:t8 0x44:rd:t1:t9 0x48:rs:t1:t9 0x48:rt:t2:t0 0x4c:rt:t2:t0 */
/* @CompileEnv: IDO52_AS1_XBB_SKIP_CAND=0 */
/* @PostProc: patch_insn_reg_field (10 fields / 7 words) */
#include "types.h"

extern s32 _SpuMemList;
extern s32 _spu_AllocLastNum;
extern s32 libspu_regs_ptr;
extern s32 _spu_AllocBlockNum;
extern s32 spu_mem_mode;

s32 SpuInitMalloc(s32 count, u32 *buf)
{
    s32 *dmaRef;
    s32 shift;

    if (count > 0) {
        _SpuMemList = (s32)buf;
        _spu_AllocLastNum = 0;
        buf[0] = 0x40001010;
        dmaRef = (s32 *)libspu_regs_ptr;
        _spu_AllocBlockNum = count;
        shift = (s32)((((u32)((u16 *)dmaRef)[214]) & 0xE) >> 1) + 1;
        buf[1] = (0x10000 << shift) - 4112;
        spu_mem_mode = shift;
        return count;
    }
    return 0;
}
