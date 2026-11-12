/* name_ent_cw103_set - NAME_ENT overlay, Address: 0x80146E88, Size: 148 bytes (37 insns) */
#include "types.h"
#include "tokimeki/class_w.h"

extern CLASS_W class_w[];

void name_ent_cw103_set(void)
{
    class_w[103].prio = 8;
    class_w[103].anim_flag = 64;
    (*(s32 *)&class_w[103].attribute) = 0x41000000;
    class_w[103].disp_flag = 164;
    (*(s32 *)&class_w[103].cels) = 0x801842C4;
    (*(s32 *)&class_w[103].anim_tbl) = 0x801842F0;
    (*(s32 *)&class_w[103].hdr) = 0x801842BC;
    class_w[103].frame = 7;
    class_w[103].tpage_sel = 128;
    class_w[103].x.half[1] = -160;
    class_w[103].y.half[1] = -120;
}
