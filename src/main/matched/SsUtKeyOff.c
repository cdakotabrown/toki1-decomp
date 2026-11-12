/* @SymbolStart: 0x8008EF34 */
/* @SymbolSize: 280 */
/* @CompileEnv: IDO52_AS1_NO_FALLTHRU_FILL=1 */
/* @CompileEnv: IDO52_AS1_NO_LA_PULL=1 */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=8:v1 */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=4:t8,5:t8 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: patch_stack_complete --old 24 --new 32 --shift-ra */
#include "types.h"

extern s32 _snd_ev_flag;
extern s16 D_8013160A;
extern s8 _svm_voice[];
extern void vmNoiseOff(s32);
extern void SpuVmKeyOffNow(s32);

s32 func_8008EF3C(s16 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4) {
    s8 *ch;

    if (_snd_ev_flag == 1) {
        return -1;
    }

    _snd_ev_flag = 1;

    if (arg0 >= 24) goto err;
    if (arg0 < 0) goto err;

    ch = _svm_voice + ((arg0 * 4 - arg0) << 3);

    if (arg1 != *(s16 *)(ch + 18)) goto err;
    if (arg2 != *(s16 *)(ch + 14)) goto err;
    if (arg3 != *(s16 *)(ch + 16)) goto err;
    if (arg4 != *(s16 *)(ch + 8)) goto err;

    if (*(s16 *)(ch + 0) == 255) {
        vmNoiseOff(arg0 & 0xFF);
    } else {
        D_8013160A = arg0;
        SpuVmKeyOffNow(0);
    }

    _snd_ev_flag = 0;
    return 0;

err:
    _snd_ev_flag = 0;
    return -1;
}
