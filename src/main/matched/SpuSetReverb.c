/* Address: 0x800986C0 */
/* @SymbolStart: 0x800986C0 */
/* @SymbolSize: 264 */
/* Size: 272 bytes (68 instructions; 66 + 2 alignment nops) */
/* Region: Library (IDO codegen band, 0x80096000-0x80098800 CD driver TU) */
/* @CompileFlags: -Wc,-pic0 */
/* @CompileEnv: IDO52_AS1_XBB_SKIP_CAND=all IDO52_AS1_NO_FALLTHRU_FILL=all IDO52_AS1_NO_JUMP_INVERT=all */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern s32 _spu_fd;
extern s32 _spu_rev_flag;
extern s32 _spu_rev_reserve_wa;
extern s32 _spu_rev_offsetaddr;

extern s32 _spu_ioctl(s32, s32, s32 *);
extern s32 _SpuIsInAllocateArea_(s32);

s32 SpuSetReverb(s32 mode)
{
    goto dispatch;
case_0:
    _spu_rev_flag = 0;
    _spu_ioctl(_spu_fd, 96, &_spu_rev_flag);
    goto done;
case_1:
    if (_spu_rev_reserve_wa == 1) {
        _spu_rev_flag = 1;
        _spu_ioctl(_spu_fd, 96, &_spu_rev_flag);
        goto done;
    }
    if (_SpuIsInAllocateArea_(_spu_rev_offsetaddr) != 0) {
        _spu_rev_flag = 0;
        _spu_ioctl(_spu_fd, 96, &_spu_rev_flag);
        goto done;
    }
    _spu_rev_flag = 1;
    _spu_ioctl(_spu_fd, 96, &_spu_rev_flag);
    goto done;
dispatch:
    if (mode == 0) goto case_0;
    if (mode == 1) goto case_1;
done:
    return _spu_rev_flag;
}
