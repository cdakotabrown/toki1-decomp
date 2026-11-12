/* name_ent_cw98_setd - NAME_ENT overlay, Address: 0x801491F8, Size: 144 bytes (36 insns) */
#include "types.h"
#include "tokimeki/class_w.h"

extern s16 cw98_anim;
extern s16 cw98_x;
extern s16 cw98_y;
extern CLASS_W class_w[];

void name_ent_cw98_setd(void)
{
    class_w[98].prio = 9;
    class_w[98].anim_flag = 17;
    (*(s32 *)&class_w[98].attribute) = 0x41000000;
    class_w[98].disp_flag = 172;
    (*(s32 *)&class_w[98].cels) = 0x801800F4;
    (*(s32 *)&class_w[98].anim_tbl) = 0x80180114;
    (*(s32 *)&class_w[98].hdr) = 0x801800F0;
    cw98_anim = 0;
    class_w[98].tpage_sel = 128;
    cw98_x = -160;
    cw98_y = -120;
}
