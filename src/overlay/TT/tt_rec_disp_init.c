/* tt_rec_disp_init - TT overlay, Address: 0x80146C04, Size: 96 bytes (24 insns) */
/* @PostProc: swap_registers v0:v1 --bidirectional */
#include "types.h"

extern u8 *tt_rec_pool;

void tt_rec_disp_init(void)
{
    u8 *p;
    s32 i;

    p = tt_rec_pool;
    i = 0;
    do {
        i += 4;
        *(u16 *)(p + 104) = 3;
        *(u16 *)(p + 152) = 10;
        *(u16 *)(p + 154) = 15489;
        *(u16 *)(p + 192) = 3;
        *(u16 *)(p + 240) = 10;
        *(u16 *)(p + 242) = 15489;
        *(u16 *)(p + 280) = 3;
        *(u16 *)(p + 328) = 10;
        *(u16 *)(p + 330) = 15489;
        p += 352;
        *(u16 *)(p - 336) = 3;
        *(u16 *)(p - 288) = 10;
        *(u16 *)(p - 286) = 15489;
    } while (i != 8);
}
