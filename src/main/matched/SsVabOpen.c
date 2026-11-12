/* Function: SsVabOpen */
/* Address: 0x80090D40 */
/* Size: 92 bytes (23 instructions) */
/* Region: Library (0x80086000+) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: patch_insn_reg_field 0x18:rd:v1:a1 0x28:rt:v1:a1 0x28:rd:v1:a1 0x2c:rt:v1:a1 0x44:rd:v1:a1 0x48:rt:v1:a1 0x48:rd:v1:a1 0x58:rs:v1:a1 */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all */
#include "types.h"

extern s32 SsVabOpenHead(s32, s32);
extern s32 SsVabTransBody(s32);
extern s32 _svm_brr_start_addr[];

s32 SsVabOpen(s32 a0, s32 a1) {
    s16 cmp_val;
    s16 val;
    a1 = SsVabOpenHead(a0, -1);
    cmp_val = (s16)a1;
    val = (s16)a1;
    if (cmp_val != -1) {
        val = (s16)SsVabTransBody(_svm_brr_start_addr[val]);
    }
    return val;
}
