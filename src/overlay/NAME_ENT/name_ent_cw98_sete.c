/* name_ent_cw98_sete - NAME_ENT overlay, Address: 0x80149608, Size: 168 bytes (42 insns) */
#include "types.h"
#include "tokimeki/class_w.h"

extern s16 cw98_anim;
extern s16 cw98_x;
extern s16 cw98_y;
extern CLASS_W class_w[];

extern void class_clear_n();

void name_ent_cw98_sete(void)
{
    class_clear_n(98);

    class_w[98].prio = 8;
    class_w[98].anim_flag = 3;
    class_w[98].attribute = 0x41000000;
    class_w[98].disp_flag = 172;
    (*(u32 *)&class_w[98].cels) = 0x80180294;
    (*(u32 *)&class_w[98].anim_tbl) = 0x801802C8;
    (*(u32 *)&class_w[98].hdr) = 0x80180284;
    cw98_anim = 0;
    class_w[98].tpage_sel = 128;
    cw98_x = -160;
    cw98_y = -120;
}
