/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: scheduler_normalizer */
/* tt_buf_clear_load - TT overlay, Address: 0x80142D30, Size: 104 bytes (26 insns) */
#include "types.h"

extern void LoadImage(void *params, u8 *obj);

void tt_buf_clear_load(u8 *obj, s16 a1, s16 a2)
{
    s16 params[4];
    u8 *p;
    s32 i;

    params[2] = 256;
    params[3] = 16;
    params[0] = a1;
    params[1] = a2;

    p = obj;
    i = 0;
    do {
        i += 128;
        *(s16 *)(p + 32) = 0;
        *(s16 *)(p + 64) = 0;
        *(s16 *)(p + 96) = 0;
        p += 128;
        *(s16 *)(p - 128) = 0;
    } while (i != 8192);

    LoadImage(params, obj);
}
