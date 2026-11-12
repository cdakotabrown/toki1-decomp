/* Address: 0x800984BC */
/* Size: 116 bytes (29 instructions) */
/* Region: Library (0x80086000+), IDO 5.2, stock pic0 */
/* @SymbolStart: 0x800984BC */
/* @SymbolSize: 116 */
/* @CompileEnv: IDO52_AS1_XBB_SKIP_CAND=all IDO52_FORCE_WEB_COLOR=10:v1,5:a1,15:v0,0:a3,20:a2,34:a0 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: patch_insn_reg_field --patch 0x0:rt:v0:a3 --patch 0x4:rs:v0:a3 */
#include "types.h"

extern s32 _spu_AllocBlockNum;
extern u32 *_SpuMemList;
extern void _gcSPU(void);

void SpuFree(u32 value) {
    s32 count;
    s32 i;
    u32 *ptr;
    u32 entry;

    count = _spu_AllocBlockNum;
    i = 0;
    if (count > 0) {
        ptr = _SpuMemList;
        do {
            entry = *ptr;
            if (entry & 0x40000000) {
                break;
            }
            if (value == entry) {
                *ptr = entry | (u32)0x80000000;
                break;
            }
            i++;
            ptr += 2;
        } while (i != count);
    }
    _gcSPU();
}
