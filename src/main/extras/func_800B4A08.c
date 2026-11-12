/* Address: 0x800B4A08 */
/* Size: 80 bytes (20 instructions) */
/* Compiler: IDO 5.2 */
/* Region: IDO (0x80081000-0x800BFFFF) */
#include "types.h"

extern void ResetCallback(void);
extern s32 PAD_init(u32 flags, s32 *ptr);
extern void ChangeClearPAD(s32 arg);

extern s32 pad_buf;

s32 func_800B4A08(void)
{
    s32 result;

    pad_buf = -1;
    ResetCallback();
    result = PAD_init(0x20000001, &pad_buf);
    ChangeClearPAD(0);
    return result;
}
