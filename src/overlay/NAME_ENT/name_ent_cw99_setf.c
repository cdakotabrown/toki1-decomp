/* name_ent_cw99_setf - NAME_ENT overlay, Address: 0x80149c20 */
#include "types.h"
#include "tokimeki/class_w.h"

extern CLASS_W class_w[];

void name_ent_cw99_setf(void)
{
    class_w[99].prio = 9;
    class_w[99].anim_flag = 5;
    class_w[99].attribute = 0x41000000;
    class_w[99].disp_flag = 164;
    (*(u32 *)&class_w[99].cels) = 0x80180258;
    (*(u32 *)&class_w[99].anim_tbl) = 0x80180298;
    (*(u32 *)&class_w[99].hdr) = 0x8018024C;
    class_w[99].tpage_sel = 128;
    class_w[99].x.half[1] = -160;
    class_w[99].y.half[1] = -120;
}
