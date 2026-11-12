/* @SymbolStart: 0x80098650 */
/* @SymbolSize: 104 */
/* @CompileFlags: -Wo,-varref */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=4:v1 IDO52_AS1_NO_JUMP_INVERT=all IDO52_AS1_XBB_SKIP_CAND=0,1,2,3,5 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
#include "types.h"

extern s32 _spu_fd;
extern s32 _spu_trans_mode;
extern void _spu_ioctl(s32 arg0, s32 arg1, s32 *arg2);

void SpuSetTransferMode(s32 arg0) {
    s32 local;

    goto dispatch;
case_0:
    local = 1;
    goto done;
case_1:
    local = 0;
    goto done;
dispatch:
    if (arg0 == 0) goto case_0;
    if (arg0 == 1) goto case_1;
    local = 1;
done:
    _spu_trans_mode = arg0;
    _spu_ioctl(_spu_fd, 33, &local);
}
