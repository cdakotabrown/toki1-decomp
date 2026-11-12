/* name_ent_pair_init - NAME_ENT overlay, Address: 0x801480ac */
#include "types.h"
#include "tokimeki/class_w.h"

extern CLASS_W class_w[];

void name_ent_pair_init(void)
{
    class_w[99].prio = 12;
    class_w[99].anim_flag = 0;
    class_w[99].attribute = 0x41000000;
    class_w[99].disp_flag = 164;
    (*(u32 *)&class_w[99].cels) = 0x80180110;
    (*(u32 *)&class_w[99].anim_tbl) = 0x80180130;
    (*(u32 *)&class_w[99].hdr) = 0x8018010C;
    class_w[99].anim = 0;
    class_w[99].tpage_sel = 128;
    class_w[99].x.half[1] = -160;
    class_w[99].y.half[1] = -120;
    class_w[100].prio = 12;
    class_w[100].anim_flag = 0;
    class_w[100].attribute = 0x41000000;
    class_w[100].disp_flag = 164;
    (*(u32 *)&class_w[100].cels) = 0x80180110;
    (*(u32 *)&class_w[100].anim_tbl) = 0x80180130;
    (*(u32 *)&class_w[100].hdr) = 0x8018010C;
    class_w[100].anim = 1;
    class_w[100].tpage_sel = 128;
    class_w[100].x.half[1] = -160;
    class_w[100].y.half[1] = -120;
}
