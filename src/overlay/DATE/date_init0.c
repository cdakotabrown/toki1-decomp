/* Function: date_init0 */
/* Overlay: DATE */
#include "types.h"

typedef struct {
    s16 angle0;
    s16 angle1;
    s16 angle2;
    s16 unk06;
} DateAngles;

typedef struct {
    s32 unk00;
    s32 unk04[3];
} Rec16;

extern s32 date_angle_set();
extern s32 date_open_sub_step_up();

extern s16 *date_move_tbl[];
extern Rec16 object[];
DateAngles date_angle_tbl[11] = {0};
extern s16 date_open_cnt;

s32 date_init0(void)
{
    s32 i;
    s32 j;

    date_angle_set(0, date_move_tbl[0], 0);
    object[0].unk00 = 0;
    date_angle_tbl[0].angle0 = 50;
    date_angle_tbl[0].angle2 = 50;
    date_angle_tbl[0].angle1 = 210;
    date_angle_set(1, date_move_tbl[1], 0);
    object[1].unk00 = 0;
    date_angle_tbl[1].angle0 = 75;
    date_angle_tbl[1].angle2 = -50;
    date_angle_tbl[1].angle1 = 195;
    for (i = 2, j = 0; i < 11; i++, j++) {
        date_angle_set(i, date_move_tbl[2], j);
        object[i].unk00 = 0;
    }
    date_open_cnt = 0;
    return date_open_sub_step_up();
}
