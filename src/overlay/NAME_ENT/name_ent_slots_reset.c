/* name_ent_slots_reset - NAME_ENT overlay, Address: 0x801496B0, Size: 196 bytes (49 insns) */
#include "types.h"
#include "tokimeki/class_w.h"

extern CLASS_W class_w[];

extern void class_clear_n();

void name_ent_slots_reset(void)
{
    class_clear_n(96);
    class_clear_n(97);
    class_clear_n(98);
    class_clear_n(99);

    class_w[99].prio = 8;
    class_w[99].anim_flag = 5;
    class_w[99].attribute = 0x41000000;
    class_w[99].disp_flag = 164;
    (*(u32 *)&class_w[99].cels) = 0x80180294;
    (*(u32 *)&class_w[99].anim_tbl) = 0x801802C8;
    (*(u32 *)&class_w[99].hdr) = 0x80180284;
    class_w[99].anim = 2;
    class_w[99].tpage_sel = 128;
    class_w[99].x.half[1] = -160;
    class_w[99].y.half[1] = -120;
}
