/* @CompileFlags: -cckr */
#include "types.h"

typedef struct {
    u8  pad0[6];
    s16 aff;
    u8  pad8[2];
    s16 stress;
    u8  pad12[2];
    s16 timer;
    union {
        struct {
            unsigned b0 : 1;
            unsigned ring : 1;
            unsigned rest : 30;
        } w;
        u8 b[4];
    } f;
    u8  pad20[36];
} Chr;

typedef struct {
    u8  pad0[440];
    Chr chr[11];
} Save;

extern Save screen_w;
extern u8 tel_ring_timeout_aff_tbl[];
extern u8 tel_ring_timeout_stress_tbl[];

void tel_ring_timeout()
{
    s32 unk1, unk2;
    s32 found;
    s32 i;

    for (i = 0; i < 11; i++) {
        if (screen_w.chr[i].f.w.ring && screen_w.chr[i].timer >= 91) {
            found = i;
            break;
        }
    }
    screen_w.chr[found].timer = 0;
    screen_w.chr[found].f.b[2] &= ~1;
    screen_w.chr[found].f.b[1] &= ~0x10;
    screen_w.chr[found].f.b[1] &= 0xDF;
    for (i = 0; i != 11; i++) {
        if (screen_w.chr[i].f.w.ring == 1) {
            screen_w.chr[i].aff -= tel_ring_timeout_aff_tbl[i];
            screen_w.chr[i].stress -= tel_ring_timeout_stress_tbl[i];
        }
    }
    sub_sub_step_up();
}
