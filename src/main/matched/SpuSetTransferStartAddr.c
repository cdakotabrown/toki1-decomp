/* Address: 0x80095620 (recorded symbol 0x80095628 is the PROLOGUE; the */
/* Size: 116 bytes (29 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 -O2 (pic0) */
/* @CompileEnv: IDO52_AS1_NO_COPY_PROP=1 IDO52_NO_ENTRY_HOME_STORE=occ:1 IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_AS1_NO_LA_PULL=all */
/* @SymbolStart: 0x80095620 */
/* @SymbolSize: 116 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern void *_spu_fd;
extern void _spu_ioctl(void *heap, s32 kind, u32 *sizep);

u32 SpuSetTransferStartAddr(u32 size)
{
    if (size >= 0x7FFF9) {
        return 0;
    }
    if ((size & 7) == 0) {
    } else {
        size = (size + 8) & ~7;
    }
    _spu_ioctl(_spu_fd, 43, &size);
    return size;
}
