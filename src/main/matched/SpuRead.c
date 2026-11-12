/* Address: 0x800956A0, Size: 80 bytes */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"
extern s32 _spu_transferCallback;
extern s32 _spu_inTransfer;
extern void _spu_read(s32 a0, u32 a1);
u32 SpuRead(s32 a0, u32 a1) {
    if (a1 >= 0x7F001u) a1 = 0x7F000u;
    _spu_read(a0, a1);
    if (_spu_transferCallback == 0) _spu_inTransfer = 0;
    return a1;
}
