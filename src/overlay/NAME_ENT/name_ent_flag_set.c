/* name_ent_flag_set - NAME_ENT overlay, Address: 0x80149D44, Size: 44 bytes (11 insns) */
#include "types.h"
#include "tokimeki/class_w.h"

extern u8 update_anim_state_tbl4;
extern CLASS_W class_w[];

void name_ent_flag_set(void)
{
    if ((class_w[100].anim_flag & 1) == 0) {
        class_w[100].anim_flag = 1;
    }
    update_anim_state_tbl4 = 15;
}
