/* Function: date_effect_move */
/* Overlay: DATE */
/* Address: 0x8013FCF8 */
/* Size: 1488 bytes (372 instructions) */
/* @CompileEnv: IDO52_NO_CSE_SITE=cvt:5,cvt:8 */
#include "types.h"

extern s32 device_cmd_send();
extern s32 dtd_on();
extern s32 _sprite_set_box_shade();
extern s32 rand();

extern volatile u8 now_sub_sub_step;
extern u32 step_counter;
typedef struct { s16 lo; s16 hi; } HL;
typedef union { s32 w; HL h; } FX;
typedef struct {
    u8 p0[2];
    u8 b2;
    u8 flags;
    u8 p4[3];
    u8 spr;
    s16 h8;
    u8 pA[12];
    s16 state;
    s16 h24;
    u8 p1A[10];
    FX pos;
    FX ht;
    s32 speed;
    u8 p30[20];
} Ent;
typedef struct { u8 pad[6460]; Ent ent[5]; } World;
extern World class_w;
extern s32 date_effect_ht[];
extern s32 date_effect_cnt;
extern s16 date_effect_num;
extern u8  date_effect_bri;

void date_effect_move(void)
{
    s16 i;
    s32 x;
    s32 dy;
    u8 selv;

    date_effect_cnt = date_effect_cnt + 1;
    if (date_effect_cnt % (rand() % 60 + 60) == 0) {
        if (date_effect_num < 4) {
            date_effect_num = date_effect_num + 1;
            class_w.ent[date_effect_num].flags =
                class_w.ent[date_effect_num].flags | 0x80;
            class_w.ent[date_effect_num].state = 3;
            class_w.ent[date_effect_num].h24 = 0;
            class_w.ent[date_effect_num].h8 = 0;
            class_w.ent[date_effect_num].b2 = 1;
            class_w.ent[date_effect_num].ht.h.hi = 160;
            class_w.ent[date_effect_num].speed =
                rand() % 32768 - 16384;
            date_effect_ht[date_effect_num - 1] =
                (-(rand() % 9) << 16) - 0x480000;
            class_w.ent[date_effect_num].pos.h.hi =
                (s16)(rand() % 256 - 128);
        }
    }
    selv = now_sub_sub_step;
    if (selv >= 38U) {
        date_effect_num = 1;
        for (i = 1; i < 4; i++) {
            class_w.ent[i + 1].flags =
                class_w.ent[i + 1].flags & 0x7F;
        }
    }
    for (i = 0; i < date_effect_num; i++) {
        if (class_w.ent[i + 1].state == 3) {
            class_w.ent[i + 1].ht.w +=
                (date_effect_ht[i] - class_w.ent[i + 1].ht.w) >> 5;
            class_w.ent[i + 1].pos.w =
                class_w.ent[i + 1].speed +
                class_w.ent[i + 1].pos.w;
            dy = class_w.ent[i + 1].ht.h.hi -
                 ((HL *)&date_effect_ht[i])->hi;
            class_w.ent[i + 1].spr = (u8)(dy / 2 + 64);
            if (dy < 10) {
                x = rand();
                selv = now_sub_sub_step;
                class_w.ent[i + 1].state =
                    class_w.ent[i + 1].state - x % 2 - 1;
                if (selv == 37 && step_counter >= 501 && i < 2) {
                    class_w.ent[i + 1].state = 0;
                }
                class_w.ent[i + 1].h24 = 0;
                class_w.ent[i + 1].h8 = 0;
                class_w.ent[i + 1].b2 = 3;
                class_w.ent[i + 1].spr = 128;
            }
        } else if (class_w.ent[i + 1].h24 == 0 &&
                   class_w.ent[i + 1].h8 == 0) {
            if ((class_w.ent[i + 1].flags & 0x80) != 0 &&
                class_w.ent[i + 1].state != 0) {
                device_cmd_send(1280);
                selv = now_sub_sub_step;
            }
            if (selv == 37 && step_counter >= 201 &&
                step_counter < 501) {
                class_w.ent[i + 1].flags =
                    class_w.ent[i + 1].flags & ~0x80;
            } else if (selv == 37 && step_counter >= 701) {
                class_w.ent[i + 1].flags =
                    class_w.ent[i + 1].flags & ~0x80;
            } else if (selv == 37 && step_counter >= 501 && i >= 2) {
                class_w.ent[i + 1].flags =
                    class_w.ent[i + 1].flags & ~0x80;
            } else {
                class_w.ent[i + 1].flags =
                    class_w.ent[i + 1].flags | 0x80;
                class_w.ent[i + 1].state = 3;
                class_w.ent[i + 1].h24 = 0;
                class_w.ent[i + 1].h8 = 0;
                class_w.ent[i + 1].b2 = 1;
                class_w.ent[i + 1].ht.h.hi = 160;
                class_w.ent[i + 1].speed =
                    rand() % 32768 - 16384;
                date_effect_ht[i] = (-(rand() % 9) << 16) - 0x280000;
                x = rand() % 256 - 128;
                selv = now_sub_sub_step;
                class_w.ent[i + 1].pos.h.hi = (s16)x;
                if (selv == 37 && step_counter >= 501) {
                    if (i == 0) {
                        class_w.ent[i + 1].pos.h.hi = -80;
                    } else if (i == 1) {
                        class_w.ent[i + 1].pos.h.hi = 80;
                    } else {
                        class_w.ent[i + 1].flags =
                            class_w.ent[i + 1].flags & 0x7F;
                    }
                }
            }
        } else if (class_w.ent[i + 1].state == 0 &&
                   class_w.ent[i + 1].h24 == 10 &&
                   class_w.ent[i + 1].h8 == 0) {
            device_cmd_send(1280);
            selv = now_sub_sub_step;
        }
    }
    if (date_effect_bri < 128U) {
        _sprite_set_box_shade(-160, -120, 320, 160, 14, date_effect_bri, 4);
    } else {
        _sprite_set_box_shade(-160, -120, 320, 160, 14, 192, 4);
    }
    dtd_on(14);
}
