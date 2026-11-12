/* Address: 0x80091080 */
/* Size: 216 bytes (54 instructions) -- the recorded 124 is truncated */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @SymbolStart: 0x80091080 */
/* @SymbolSize: 216 */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_AS1_NO_LA_PULL=all IDO52_AS1_NO_FALLTHRU_FILL=all */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u8   _svm_vab_used[];
extern s16  kMaxPrograms;
extern s32  _svm_vab_vh[];
extern u8  *_svm_vab_pg[];
extern s32  _svm_vab_tn[];
extern s32  _svm_vh;
extern u8  *_svm_pg;
extern s32  _svm_tn;
extern u8   libsnd_cur_vab_id;
extern u8   libsnd_cur_program;
extern u8   libsnd_cur_fake_program;

s32 SpuVmVSetUp(s16 slot, s16 n) {
    if (slot >= 16 || slot < 0 || _svm_vab_used[slot] != 1 || n >= kMaxPrograms) {
        return -1;
    }
    _svm_vh = _svm_vab_vh[slot];
    _svm_pg = _svm_vab_pg[(u32)slot];
    _svm_tn = *(s32 *)((u8 *)_svm_vab_tn + (slot << 2));
    libsnd_cur_vab_id = slot;
    libsnd_cur_program = n;
    libsnd_cur_fake_program = *(s32 *)((_svm_pg + 8) + n * 16);
    return 0;
}
