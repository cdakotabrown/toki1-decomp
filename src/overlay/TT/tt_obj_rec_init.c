/* tt_obj_rec_init - TT overlay, Address: 0x8013BDC8, Size: 64 bytes (16 insns) */
#include "types.h"

extern s32 tt_obj_anim_tbl;

void tt_obj_rec_init(u8 *obj)
{
    obj[0] = 1;
    *(s16 *)(obj + 2) = 32;
    *(s16 *)(obj + 82) = 10;
    *(s16 *)(obj + 84) = 15492;
    *(s32 *)(obj + 56) = tt_obj_anim_tbl;
    obj[64] = 128;
    *(s16 *)(obj + 16) = 6;
}
