/* Function: SsVabTransBody */
/* Address: 0x8008C5F0 */
/* Size: 224 bytes (56 instructions) */
/* Region: Library (0x80086000+) */
/* cracked.  `@CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all` (Hook 41b) */
/* @CompileEnv: IDO52_AS1_NO_LA_PULL=all */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all */
/* @PostProc: convert_or_to_addu */
/* @PostProc: swap_instructions --offset1 0x74 --offset2 0x78 */
/* @PostProc: remap_stack_slots --map 24:32,28:36,32:40,36:44,48:52 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u8 _svm_vab_used;
extern s32 _svm_vab_start;
extern s32 _svm_vab_total;

extern s32 SpuSetTransferMode(s32);
extern s32 SpuSetTransferStartAddr(s32);
extern s32 SpuWrite(s32, s32);

s32 SsVabTransBody(s32 arg0, s16 idx) {
    u8 *entry;
    s32 target;
    s32 *tbl;

    if (idx >= 17 || idx < 0) {
        return -1;
    }
    entry = &_svm_vab_used + idx;
    if (*entry != 2) {
        return -1;
    }
    target = *(&_svm_vab_start + idx);
    SpuSetTransferMode(0);
    SpuSetTransferStartAddr(target);
    tbl = &_svm_vab_total + idx;
    if (SpuWrite(arg0, *tbl) == *tbl) {
        goto ok;
    }
    return -1;
ok:
    *entry = 1;
    return idx;
}
