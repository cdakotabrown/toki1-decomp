/* @SymbolSize: 196 */
/* name_ent_cw101_set - NAME_ENT overlay, Address: 0x80146DC4, Size: 196 bytes (49 insns) */
#include "types.h"
#include "tokimeki/class_w.h"

extern s32 get_CD_State();

extern CLASS_W class_w[];

void name_ent_cw101_set(void)
{
    s32 buttons;

    buttons = get_CD_State();

    if (buttons & 0x0004) {
        class_w[101].prio = 8;
        class_w[101].anim_flag = 1;
        (*(s32 *)&class_w[101].attribute) = 0x41000000;
        class_w[101].disp_flag = 164;
        (*(s32 *)&class_w[101].cels) = 0x801800F8;
        (*(s32 *)&class_w[101].anim_tbl) = 0x80180114;
        (*(s32 *)&class_w[101].hdr) = 0x801800F4;
        class_w[101].anim = 2;
        class_w[101].tpage_sel = 128;
        class_w[101].x.half[1] = -160;
        class_w[101].y.half[1] = -120;
    } else {
        class_w[101].anim_flag = 0;
    }
}
