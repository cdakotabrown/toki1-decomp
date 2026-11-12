/* @CompileFlags: -cckr */
#include "types.h"

typedef struct {
    u8  unk00[12];
    u8  unk0C;
    u8  unk0D;
    u8  unk0E;
    u8  unk0F;
} FadePal;

typedef struct {
    s32 unk00;
    s32 unk04;
    s32 unk08;
    s32 unk0C;
} ObjRec;

extern FadePal pslt[];
extern ObjRec  object_2[];
extern u8  back_clear_b;
extern s32 date_move_tbl[];
extern s16 date_open_cnt;

void message_window_show();
void basyo_show();
void update_anim_state();
void GsSetFlatLight();
void date_angle_set();
void date_open_sub_step_up();

date_open_main()
{
    s32 t;
    s32 i;
    s16 c;
    s32 j;

    message_window_show();
    update_anim_state(2);
    basyo_show();

    t = date_open_cnt;
    if (t < 32) {
        if (pslt[0].unk0C < 192) {
            pslt[0].unk0C = pslt[0].unk0D = pslt[0].unk0E += 6;
            pslt[1].unk0C = pslt[1].unk0D = pslt[1].unk0E += 1;
            pslt[2].unk0C = pslt[2].unk0D = pslt[2].unk0E += 1;
            GsSetFlatLight(0, &pslt[0]);
            GsSetFlatLight(1, &pslt[1]);
            GsSetFlatLight(2, &pslt[2]);
        }
        if (t < 15) {
            back_clear_b++;
        }
    } else if (t >= 32 && t < 84) {
        for (i = 0; i < 2; i++) date_angle_set(i, date_move_tbl[i], 1);
    } else if (t >= 84 && t < 104) {
        for (i = 0; i < 2; i++) date_angle_set(i, date_move_tbl[i], 2);
    } else if (t >= 104 && t < 114) {
        for (i = 0; i < 2; i++) date_angle_set(i, date_move_tbl[i], 3);
    } else if (t >= 114 && t < 124) {
        for (i = 0; i < 2; i++) date_angle_set(i, date_move_tbl[i], 4);
    } else if (t >= 124 && t < 154) {
        for (i = 0; i < 2; i++) date_angle_set(i, date_move_tbl[i], 5);
    } else if (t >= 154 && t < 174) {
        for (i = 0; i < 2; i++) date_angle_set(i, date_move_tbl[i], 6);
    } else if (t >= 174 && t < 234) {
        for (i = 0; i < 2; i++) date_angle_set(i, date_move_tbl[i], 7);
    } else if (t >= 270 && t < 302) {
        if (pslt[0].unk0C != 0) {
            pslt[0].unk0C = pslt[0].unk0D = pslt[0].unk0E -= 6;
            pslt[1].unk0C = pslt[1].unk0D = pslt[1].unk0E -= 1;
            pslt[2].unk0C = pslt[2].unk0D = pslt[2].unk0E -= 1;
            GsSetFlatLight(0, &pslt[0]);
            GsSetFlatLight(1, &pslt[1]);
            GsSetFlatLight(2, &pslt[2]);
        }
        if (t < 285) {
            back_clear_b--;
        }
        if (pslt[0].unk0C == 0) {
            for (i = 0; i < 9; i++) {
                object_2[i].unk00 = 0x80000000;
            }
        }
    } else if (t >= 312) {
        date_open_sub_step_up();
    }

    for (i = 2, j = 9; i < 11; i++, j++) {
        date_angle_set(i, date_move_tbl[2], j);
    }
    c = date_open_cnt + 1;
    date_open_cnt = c;
}
