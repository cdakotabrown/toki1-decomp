/* Function: SpuClearReverbWorkArea */
/* Address: 0x80095310 */
/* Size: 500 bytes (125 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 -O2, pic0 */
/* @SymbolSize: 500 */
/* @CompileFlags: -Wo,-do_opt_saved_regs */
/* @CompileEnv: IDO52_LDA_NOINREG_CLASSES=1,4:1:2 IDO52_AS1_XBB_SKIP_CAND=all IDO52_AS1_NO_FALLTHRU_FILL=all IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_FORCE_WEB_COLOR=19:v1 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
#include "types.h"

typedef struct {
    u8 pad[0x1AC];
    u16 field_1AC;
} SomeStruct;

extern SomeStruct *libspu_revclear_regs_ptr;
extern u32 _spu_rev_startaddr[10];
extern u32 _spu_fd;
extern u8 libspu_zero_fill_buf[1024];
extern u32 _spu_EVdma;
extern volatile s32 _spu_transferCallback;

extern s32 _SpuIsInAllocateArea_(u32 arg0);
extern void _spu_ioctl(u32 arg0, s32 arg1, u32 *arg2);
extern void _spu_write(u8 *arg0, u32 arg1);
extern void WaitEvent(u32 arg0);

s32 SpuClearReverbWorkArea(s32 arg0) {
    u32 shift;
    u32 startAddr;
    u32 remain;
    s32 more;
    u32 *tablePtr;
    u32 status;
    u32 one;
    s32 restore;
    volatile s32 saved;
    u32 chunkSize;

    shift = libspu_revclear_regs_ptr->field_1AC;
    shift &= 0xE;
    shift >>= 1;

    restore = 0;
    saved = 0;
    shift += 1;

    if (arg0 >= 10 || arg0 < 0) {
        return -1;
    }

    tablePtr = &_spu_rev_startaddr[arg0];

    if (_SpuIsInAllocateArea_(*tablePtr) != 0) {
        return -1;
    }

    if (tablePtr == &_spu_rev_startaddr[0]) {
        remain = 0x10 << shift;
        startAddr = 0xFFF0 << shift;
    } else {
        remain = (0x10000 - *tablePtr) << shift;
        startAddr = *tablePtr << shift;
    }

    _spu_ioctl(_spu_fd, 0x2F, &status);
    if (status == 0) {
        one = 1;
        _spu_ioctl(_spu_fd, 0x2E, &one);
        restore = 1;
    }

    more = 1;
    if (_spu_transferCallback != 0) {
        saved = _spu_transferCallback;
        _spu_transferCallback = 0;
    }

    while (more != 0) {
        if (remain >= 0x401) {
            chunkSize = 0x400;
        } else {
            chunkSize = remain;
            more = 0;
        }
        _spu_ioctl(_spu_fd, 0x2B, &startAddr);
        _spu_write(libspu_zero_fill_buf, chunkSize);
        WaitEvent(_spu_EVdma);
        remain -= 0x400;
        startAddr += 0x400;
    }

    if (restore != 0) {
        _spu_ioctl(_spu_fd, 0x2E, &status);
    }
    if (saved != 0) {
        _spu_transferCallback = saved;
    }
    return 0;
}
