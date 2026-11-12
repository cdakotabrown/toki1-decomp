/* name_ent_cw100_set - NAME_ENT overlay, Address: 0x80146D00, Size: 196 bytes (49 insns) */
#include "types.h"
#include "tokimeki/class_w.h"

extern s32 get_CD_State(void);

extern CLASS_W class_w[];

void name_ent_cw100_set(void)
{
    s32 result;

    result = get_CD_State();
    if (result & 0x04) {
        class_w[100].prio = 8;
        class_w[100].anim_flag = 1;
        (*(s32 *)&class_w[100].attribute) = 0x41000000;
        class_w[100].disp_flag = 164;
        (*(s32 *)&class_w[100].cels) = 0x801800F8;
        (*(s32 *)&class_w[100].anim_tbl) = 0x80180114;
        (*(s32 *)&class_w[100].hdr) = 0x801800F4;
        class_w[100].anim = 1;
        class_w[100].tpage_sel = 128;
        class_w[100].x.half[1] = -160;
        class_w[100].y.half[1] = -120;
    } else {
        class_w[100].anim_flag = 0;
    }
}
