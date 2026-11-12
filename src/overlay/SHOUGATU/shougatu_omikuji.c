/* @RodataBase: 0x80143770 */
/* @CompileFlags: -cckr */
#include "types.h"

extern void set_kanji_string();
extern void device_cmd_send();
extern void sprintf();
extern void shougatu_omikuji_set();
extern void sub_sub_step_up();
extern s32 check_end_k();
extern void k_reset();
extern void mes_arrow_anim();

typedef struct { u8 b[21]; } BlkA;
typedef struct { u8 b[23]; } BlkB;
typedef struct { u8 b[15]; } BlkC;
typedef struct { u8 b[35]; } BlkD;

extern BlkA shougatu_omikuji_tbl0;
extern BlkB shougatu_omikuji_tbl1;
extern BlkC shougatu_omikuji_tbl2;
extern BlkD shougatu_omikuji_tbl3;

extern u8 shougatu_omikuji_dots_fmt[];
extern u8 shougatu_omikuji_oh_fmt[];
extern u8 shougatu_omikuji_lucky_fmt[];

typedef union {
    s32 l;
    struct { u16 lo; s16 hi; } w;
} Fix;

typedef struct {
    u8 b00;
    u8 b01;
    u8 state;
    u8 flags;
    u8 pad04[4];
    s16 h08;
    u8 pad0A[12];
    s16 h16;
    s16 h18;
    u8 pad1A[10];
    Fix x;
    Fix y;
    Fix vx;
    Fix vy;
    u8 pad34[16];
} Rec68;

extern Rec68 class_w[];

extern s32 step_counter;
extern u8 step_mode;
extern u8 voice_level;
extern u8 now_year;
extern s32 pad_trig;
extern s16 sound_seek_time;
extern s32 serifu_state;
extern s32 serifu_voice;
extern u8 shougatu_omikuji_no;
extern u8 shougatu_omikuji_box_on;

#define OBJ(n) class_w[n]

s32 shougatu_omikuji()
{
    s32 i;
    BlkA bufA;
    BlkB bufB;
    BlkC bufC;
    BlkD bufD;

    bufA = shougatu_omikuji_tbl0;
    bufB = shougatu_omikuji_tbl1;
    bufC = shougatu_omikuji_tbl2;
    bufD = shougatu_omikuji_tbl3;

    switch (step_counter) {
    case 0:
        set_kanji_string(-104, 48, 0, &bufA, 0);
        serifu_state = 1;
        OBJ(96).vx.w.hi = -4;
        step_counter++;
        return 0;
    case 2:
        set_kanji_string(-104, 48, 0, &bufB, 0);
        serifu_state = 1;
        shougatu_omikuji_box_on = 0;
        OBJ(99).flags |= 0x80;
        OBJ(99).state = 1;
        OBJ(99).h16 = 1;
        OBJ(99).h18 = 0;
        step_counter++;
        return 0;
    case 3:
        OBJ(96).x.l += OBJ(96).vx.l;
        if (OBJ(96).x.w.hi < -111) {
            OBJ(99).h16 = 0;
            OBJ(96).vx.w.hi = -OBJ(96).vx.w.hi;
            OBJ(96).vx.w.hi += 2;
            if (OBJ(96).vx.w.hi > 20) {
                OBJ(96).vx.w.hi = 20;
            }
        } else if (OBJ(96).x.w.hi >= 112) {
            OBJ(99).h16 = 1;
            OBJ(96).vx.w.hi = -OBJ(96).vx.w.hi;
            OBJ(96).vx.w.hi -= 2;
            if (OBJ(96).vx.w.hi < -20) {
                OBJ(96).vx.w.hi = -20;
            }
        }
        if (OBJ(96).vx.w.hi < -8 || OBJ(96).vx.w.hi > 8) {
            OBJ(97).h08++;
            OBJ(98).h08++;
        }
        if (OBJ(96).vx.w.hi < -12 || OBJ(96).vx.w.hi > 12) {
            OBJ(97).h08 += 2;
            OBJ(98).h08 += 2;
        }
        if (OBJ(96).vx.w.hi < -16 || OBJ(96).vx.w.hi > 16) {
            OBJ(97).h08 += 4;
            OBJ(98).h08 += 4;
        }
        break;
    case 4:
        if ((OBJ(96).x.w.hi >= 0 && OBJ(96).x.w.hi <= -OBJ(96).vx.w.hi) ||
            (OBJ(96).x.w.hi <= 0 && OBJ(96).x.w.hi >= -OBJ(96).vx.w.hi)) {
            OBJ(96).x.w.hi = 0;
            shougatu_omikuji_box_on = 1;
            OBJ(99).flags &= 0x7F;
            step_counter++;
        } else {
            OBJ(96).x.l += OBJ(96).vx.l;
            if (OBJ(96).x.w.hi < -111) {
                OBJ(96).vx.w.hi = 10;
            }
            if (OBJ(96).x.w.hi >= 112) {
                OBJ(96).vx.w.hi = -10;
            }
        }
        return 0;
    case 5:
        if (step_mode == 0) {
            OBJ(96).vx.l = 0;
            OBJ(96).vy.l = 0xFFFC0000;
            OBJ(96).flags |= 0x10;
        }
        OBJ(97).state = 0;
        OBJ(98).state = 0;
        if (OBJ(96).y.w.hi < -180) {
            OBJ(96).y.w.hi = -180;
        }
        if (step_mode++ == 30) {
            OBJ(96).vy.l = 0;
            OBJ(96).h16 = (now_year == 0x61) + ((now_year == 0x62) << 2) + 1;
            OBJ(96).state = 5;
        }
        if (step_mode == 75) {
            shougatu_omikuji_box_on = 0;
            OBJ(96).flags &= 0x7F;
            OBJ(99).flags |= 0x80;
            OBJ(99).state = 9;
            OBJ(99).h16 = 2;
            OBJ(99).h18 = 0;
            device_cmd_send(1280);
        }
        if (step_mode < 135) {
            return 0;
        }
        shougatu_omikuji_set();
        set_kanji_string(-104, 48, 0, &bufC, 0);
        step_counter++;
        return 0;
    case 6:
        if ((OBJ(96).state != 1) | 8) {
            step_counter++;
            serifu_state = 1;
        }
        return 0;
    case 8:
        OBJ(96).flags |= 0x80;
        OBJ(96).state = 64;
        OBJ(96).h18 = shougatu_omikuji_no + 13;
        OBJ(96).x.w.hi = -160;
        OBJ(96).y.w.hi = -120;
        OBJ(96).vy.l = 0;
        if (shougatu_omikuji_no == 3 || shougatu_omikuji_no == 4) {
            sprintf(&bufB, shougatu_omikuji_dots_fmt, &bufD.b[shougatu_omikuji_no * 5]);
            OBJ(99).flags |= 0x80;
            OBJ(99).state = 64;
            OBJ(99).h18 = 7;
        } else if (shougatu_omikuji_no != 0) {
            sprintf(&bufB, shougatu_omikuji_oh_fmt, &bufD.b[shougatu_omikuji_no * 5]);
            OBJ(99).h18 = 6;
            shougatu_omikuji_box_on = 1;
            OBJ(99).flags &= 0x7F;
        } else {
            sprintf(&bufB, shougatu_omikuji_lucky_fmt, &bufD.b[shougatu_omikuji_no * 5]);
            OBJ(99).h18 = 6;
            shougatu_omikuji_box_on = 1;
            OBJ(99).flags &= 0x7F;
        }
        set_kanji_string(-104, 48, 0, &bufB, 0);
        serifu_state = 1;
        step_counter++;
        return 0;
    default:
        sub_sub_step_up();
        break;
    case 1:
    case 7:
    case 9:
        break;
    }

    if (serifu_state != 0) {
        if (pad_trig & 0x400040) {
            if (check_end_k()) {
                if (voice_level == 2) {
                    k_reset(1);
                    serifu_state = 0;
                    serifu_voice = 0;
                    sound_seek_time = 0;
                    OBJ(2).flags &= 0xFF7F;
                    step_counter++;
                    return 1;
                }
            }
        }
        if (pad_trig & 0x200020) {
            if (check_end_k()) {
                k_reset(1);
                serifu_state = 0;
                serifu_voice = 0;
                sound_seek_time = 0;
                OBJ(2).flags &= 0xFF7F;
                step_counter++;
                return 1;
            }
        }
        if (check_end_k()) {
            OBJ(2).flags |= 0x80;
            mes_arrow_anim();
        } else {
            OBJ(2).flags &= 0x7F;
        }
    }
}
