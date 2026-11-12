/* @CompileFlags: -cckr */
/* @RodataBase: 0x8014D460 */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=1:t7,2:t8,3:t9,4:t0,5:t1,6:t2,7:t3,8:t4,9:t5,10:t6,11:t7,12:t8,13:t9,14:t0,15:t1,16:t2,17:t3,18:t4,19:t5 */
#include "types.h"
#include "tokimeki/tt_box.h"

extern s32 tt_obj_anim_tbl[];
extern TT_BOX tt_hit_box_tbl[];

void tt_shot_sprite_set(p, a1, a2)
u8 *p;
int a1;
int a2;
{
    *(s8 *)p = 1;
    *(s16 *)(p + 2) = a1;
    *(s16 *)(p + 16) = 1;
    *(s32 *)(p + 56) = tt_obj_anim_tbl[a2];
    p[64] = 128;
    switch (a2) {
    case 18:
    case 19:
        *(s16 *)(p + 82) = 10;
        *(s16 *)(p + 84) = 15499;
        *(u8 **)(p + 76) = ((u8 *)&tt_hit_box_tbl[3].x);
        break;
    case 20:
        *(s16 *)(p + 82) = 10;
        *(s16 *)(p + 84) = 15500;
        *(u8 **)(p + 76) = ((u8 *)&tt_hit_box_tbl[3].x);
        break;
    case 21:
        *(s16 *)(p + 82) = 18;
        *(s16 *)(p + 84) = 15571;
        *(u8 **)(p + 76) = ((u8 *)&tt_hit_box_tbl[3].x);
        break;
    case 22:
        *(s16 *)(p + 82) = 18;
        *(s16 *)(p + 84) = 15572;
        *(u8 **)(p + 76) = ((u8 *)&tt_hit_box_tbl[2].x);
        break;
    case 23:
        *(s16 *)(p + 82) = 18;
        *(s16 *)(p + 84) = 15573;
        *(u8 **)(p + 76) = ((u8 *)&tt_hit_box_tbl[2].x);
        break;
    }
}
