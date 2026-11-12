/* name_ent_obj2_init - NAME_ENT overlay, Address: 0x80149ca8 */
#include "types.h"
#include "tokimeki/class_w.h"

extern u8 update_anim_state_tbl4;
extern CLASS_W class_w[];

void name_ent_obj2_init(void)
{
    class_w[100].prio = 10;
    class_w[100].anim_flag = 0;
    class_w[100].attribute = 0x41000000;
    class_w[100].disp_flag = 164;
    (*(u32 *)&class_w[100].cels) = 0x80180258;
    (*(u32 *)&class_w[100].anim_tbl) = 0x80180298;
    (*(u32 *)&class_w[100].hdr) = 0x8018024C;
    class_w[100].anim = 3;
    class_w[100].tpage_sel = 128;
    class_w[100].x.half[1] = -160;
    class_w[100].y.half[1] = -56;
    update_anim_state_tbl4 = 9;
}
