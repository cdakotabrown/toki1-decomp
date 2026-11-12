/* Address: 0x8013B6C8, Size: 608 bytes (152 instructions) */
#include "types.h"

typedef struct {
    u8   pad00[6];
    u16  mode;
    u8   pad08[12];
    s16  x;
    s16  y;
    u8   pad18[704];
    s16  unk2D8;
    s16  unk2DA;
    s16  unk2DC;
    s16  unk2DE;
    u8   pad2E0[132];
    u16  unk364;
    u16  unk366;
    u16  unk368;
    u8   pad36A[62];
    s16  unk3A8;
    s16  unk3AA;
} Rec;

extern Rec *tt_boss_wk;

extern void tt_cmd_once(s32);
extern void tt_shot_aimed_make(s32, s32, s32, s32);
extern void tt_shot_spread_make(s32, s32, s32, s32, s32);
extern void tt_shot_fan_make(s32, s32, s32, s32, s32);
extern void tt_shot_polar_make(s32, s32, s32, s32);

void tt_match_shot_step(void)
{
    Rec *s;

    s = tt_boss_wk;

    switch (s->mode) {
    case 0:
        switch (tt_boss_wk->unk364) {
        case 0:
            s->unk3A8 = 2;
            s->unk3AA = 2;
            break;
        case 62:
            s->unk3A8 = 1;
            s->unk3AA = 1;
            break;
        case 64:
            s->unk3A8 = 0;
            s->unk3AA = 0;
            tt_shot_aimed_make(s->x, s->y + 56, 24, 168);
            if (s->unk2DC >= 0) {
                tt_shot_fan_make(s->x + 38, s->y + 62, 256, 24, 168);
            }
            if (s->unk2DE >= 0) {
                tt_shot_fan_make(s->x - 38, s->y + 62, 256, 24, 168);
            }
            break;
        case 72:
            s->unk3A8 = 1;
            s->unk3AA = 1;
            break;
        case 78:
            s->unk3A8 = 2;
            s->unk3AA = 2;
            break;
        }
        if (s->unk364 >= 128U) {
            s->unk364 = 0;
        }
        break;
    case 2:
        if (tt_boss_wk->unk364 >= 64U) {
            s->unk364 = 0;
            tt_shot_spread_make(s->x, s->y + 56, 256, 32, 168);
        }
        if (s->unk368 >= 8U) {
            if (s->unk2D8 >= 0 || s->unk2DA >= 0) {
                tt_cmd_once(1292);
            }
            s->unk368 = 0;
            if (s->unk2D8 >= 0) {
                tt_shot_polar_make(s->x + 56, s->y + 16, 32, 1024);
            }
            if (s->unk2DA >= 0) {
                tt_shot_polar_make(s->x - 56, s->y + 16, 32, 1024);
            }
            break;
        }
        break;
    }

    s->unk364 = s->unk364 + 1;
    s->unk366 = s->unk366 + 1;
    s->unk368 = s->unk368 + 1;
}
