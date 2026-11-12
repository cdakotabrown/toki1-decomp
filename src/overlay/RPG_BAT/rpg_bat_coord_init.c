/* @SymbolSize: 452 */
/* rpg_bat_coord_init - RPG_BAT overlay, Address: 0x80141A64, Size: 452 bytes (113 insns) */
#include "types.h"
#include "tokimeki/class_w.h"
extern CLASS_W class_w[];

extern s32 rpg_bat_anim_set();


void rpg_bat_coord_init(void)
{
    class_w[124].prio = 1;
    rpg_bat_anim_set(31, 0, 1, 1);
    rpg_bat_anim_set(38, 1, 1, 1);
    rpg_bat_anim_set(35, 4, 1, 0);
    class_w[112].x.half[1] = 107;
    class_w[112].y.half[1] = 59;
    class_w[116].x.half[1] = 107;
    class_w[116].y.half[1] = 75;
    class_w[120].x.half[1] = 107;
    class_w[120].y.half[1] = 91;
    class_w[113].x.half[1] = 116;
    class_w[113].y.half[1] = 59;
    class_w[117].x.half[1] = 116;
    class_w[117].y.half[1] = 75;
    class_w[121].x.half[1] = 116;
    class_w[121].y.half[1] = 91;
    class_w[114].x.half[1] = 125;
    class_w[114].y.half[1] = 59;
    class_w[118].x.half[1] = 125;
    class_w[118].y.half[1] = 75;
    class_w[122].x.half[1] = 125;
    class_w[122].y.half[1] = 91;
    class_w[115].x.half[1] = 134;
    class_w[115].y.half[1] = 59;
    class_w[119].x.half[1] = 134;
    class_w[119].y.half[1] = 75;
    class_w[123].x.half[1] = 134;
    class_w[123].y.half[1] = 91;
    class_w[129].x.half[1] = 88;
    class_w[129].y.half[1] = 80;
    class_w[129].anim = 8;
    class_w[128].x.half[1] = 88;
    class_w[128].y.half[1] = 96;
    class_w[128].anim = 8;
}
