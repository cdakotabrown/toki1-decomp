/* Address: 0x8008F394 */
/* Size: 128 bytes (32 instructions) */
/* Region: Library (0x80086000+) */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_AS1_XBB_SKIP_CAND=0 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: patch_insn_reg_field 0xc:rd:a1:v1 0x14:rt:a1:v1 0x14:rd:a1:v1 0x34:rs:a1:v1 0x40:rs:a1:v1 0x48:rt:a1:v1 */
#include "types.h"

extern s32 _snd_ev_flag;
extern s16 D_8013160A;
extern s32 SpuVmKeyOffNow(s32);

s32 SsUtKeyOffV(s16 arg0) {
    if (_snd_ev_flag != 1) {
        goto work;
    }
    return -1;

work:
    _snd_ev_flag = 1;
    if (arg0 < 24 && arg0 >= 0) {
        D_8013160A = arg0;
        SpuVmKeyOffNow(0);
        _snd_ev_flag = 0;
        return 0;
    }
    _snd_ev_flag = 0;
    return -1;
}
