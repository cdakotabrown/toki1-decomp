/* @CompileFlags: -cckr */
#include "types.h"

typedef struct {
    u8  pad0;
    u8  f1;
    u8  f2;
    u8  f3;
    u8  f4;
    u8  f5;
    u8  pad6;
    u8  f7;
    u8  pad8[4];
    s32 f12;
    s32 f16;
    u8  pad20[4];
    s16 f24;
    s16 f26;
    s16 f28;
    s16 pad30[4];
    s16 f38;
    s16 pad40;
    s16 f42;
    u8  pad44[8];
    s32 f52;
    s32 f56;
    u8  pad60[7];
    u8  f67;
} Rec;

extern Rec cw96_active[];
extern u8  screen_bri;
extern u8  step_mode;
extern s32 option_bg_flag;
void palette_upload_enqueue(s32 a, u8 b, s32 c, s32 d, s32 e);
void class_clear_f();
void white_light(s32 v);

option_init1()
{
    s32 i;

    if (step_mode == 0) {
        if (get_CD_State2() & 1) {
            step_mode = 1;
        }
    } else if (step_mode == 1) {
        class_clear_f();
        option_bg_flag = 1;
        for (i = 0; i < 64; i++) {
            cw96_active[i].f1 = 5;
            cw96_active[i].f2 = 64;
            cw96_active[i].f56 = 0x01000000;
            cw96_active[i].f3 = 36;
            cw96_active[i].f12 = 0x801C32D8;
            cw96_active[i].f16 = 0x801C34E0;
            cw96_active[i].f52 = 0x801C32C8;
            cw96_active[i].f4 = 8;
            cw96_active[i].f24 = 0;
            cw96_active[i].f67 = 0;
            cw96_active[i].f5 = 128;
            cw96_active[i].f38 = -512;
            cw96_active[i].f42 = -512;
            cw96_active[i].f7 = 128;
        }
        for (i = 0; i < 4; i++) {
            palette_upload_enqueue(0x801C34F0, i + 16, 1, 1, i);
        }
        palette_upload_enqueue(0x801B0000, 31, 1, 1, 0);
        for (i = 0; i < 4; i++) {
            palette_upload_enqueue(0x801CBEF0, i + 26, 1, 1, i);
        }
        load_tpage_buf_lock(5, 0x801C7CF0, 1);
        option_bg_init();
        cw96_active[0].f24 = 0;
        cw96_active[0].f38 = -160;
        cw96_active[0].f42 = -120;
        cw96_active[0].f3 |= 0x80;
        cw96_active[0].f1 = 8;
        cw96_active[1].f24 = 10;
        cw96_active[1].f38 = -160;
        cw96_active[1].f42 = -120;
        cw96_active[1].f3 |= 0x80;
        cw96_active[1].f1 = 3;
        cw96_active[10].f24 = 7;
        cw96_active[10].f38 = -160;
        cw96_active[10].f42 = -120;
        cw96_active[10].f3 = 36;
        cw96_active[10].f1 = 1;
        cw96_active[63].f24 = 7;
        cw96_active[63].f38 = -160;
        cw96_active[63].f42 = -120;
        cw96_active[63].f3 = 36;
        cw96_active[63].f1 = 1;
    }
    if (step_mode != 0) {
        screen_bri -= 4;
        if (screen_bri < 129) {
            screen_bri = 128;
            if (step_mode == 1) {
                step_mode = 2;
            }
        }
        if (step_mode == 2) {
            sub_sub_step_up();
        }
    }
    white_light(screen_bri);
}
