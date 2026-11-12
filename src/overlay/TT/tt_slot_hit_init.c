/* Address: 0x8014C570, Size: 120 bytes (30 insns) */
#include "types.h"

extern s32 tt_cmd_once(s32 soundId);
extern u8 tt_anim_tbl[];

void tt_slot_hit_init(u8 *obj)
{
    tt_cmd_once(1548);

    *(s32 *)(obj + 40) = 0;
    *(s32 *)(obj + 44) = 0;
    *(u8 **)(obj + 56) = tt_anim_tbl;
    obj[60] = 0;
    obj[61] = 2;
    obj[62] = 9;
    obj[64] = 32;
    *(s16 *)(obj + 110) = 0;
    *(s16 *)(obj + 10) = 0;
    *(s16 *)(obj + 8) = 0;
    *(s16 *)(obj + 6) = 0;
    obj[15] = 0;
    obj[14] = 0;
    obj[13] = 0;
}
