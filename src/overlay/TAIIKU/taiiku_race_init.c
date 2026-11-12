/* Address: 0x801355EC  Size: 568 bytes (142 insns) */
#include "types.h"
#include "tokimeki/taiiku_kari_run.h"

typedef struct { s16 h[11]; } RecA;

typedef struct {
    u8 b0;
    u8 b1;
    u8 b2;
    u8 b3;
    u8 b4;
    u8 b5;
    u8 b6;
    u8 b7;
    s16 h8;
    s16 h10;
    s16 h12;
    s16 h14;
    s32 w16;
    s32 w20;
    s32 w24;
    s32 w28;
    s16 h32;
    s16 h34;
} Rec36;

extern void srand(s32 a0);

extern RecA taiiku_race_init_tbl;
extern s16  taiiku_kari_gl_card[];
extern Rec36 taiiku_kari_run[];
extern u8   taiiku_kari_rank[];
extern s16  taiiku_kari_btn;
extern u8   taiiku_kari_cue;
extern s32  taiiku_kari_scrv;
extern s32  taiiku_kari_scrl;
extern s16  taiiku_kari_phase;
extern volatile s16 para_un;
extern s32  taiiku_kari_gain;
extern s32  taiiku_kari_vmax;
extern s32  vblnk_counter;
extern u8   taiiku_kari_hop[];
u8 taiiku_kari_hop_tmr[2] = {0};
extern s32  taiiku_konami_mode;
extern u8   taiiku_kari_girl;

void taiiku_race_init(void) {
    s32 i;
    s32 v0;
    RecA local;

    local = taiiku_race_init_tbl;
    for (i = 0; i < 11; i++) {
        taiiku_kari_gl_card[i] = local.h[i];
    }

    for (i = 0; i < 4; i++) {
        taiiku_kari_rank[i] = 0;
        taiiku_kari_run[i].h34 = i * 8;
        taiiku_kari_run[i].b0 = 0;
        taiiku_kari_run[i].b1 = 0;
        taiiku_kari_run[i].b2 = 0;
        taiiku_kari_run[i].b6 = 0;
        taiiku_kari_run[i].b3 = 0;
        taiiku_kari_run[i].b4 = 0;
        taiiku_kari_run[i].h12 = 0;
        taiiku_kari_run[i].h14 = 0;
        taiiku_kari_run[i].w16 = 0;
        taiiku_kari_run[i].w20 = 0;
        taiiku_kari_run[i].w24 = 0;
        taiiku_kari_run[i].w28 = 0;
        taiiku_kari_run[i].h32 = 0;
        taiiku_kari_run[i].b5 = 0;
        taiiku_kari_run[i].h8 = 0;
        taiiku_kari_run[i].h10 = 0;
        taiiku_kari_run[i].b7 = 0;
    }

    ((TAIIKU_KARI_RUN *)taiiku_kari_run)[0].pos.half[1] = -160;
    taiiku_kari_btn = 0;
    taiiku_kari_cue = 0;
    taiiku_kari_scrv = 0;
    taiiku_kari_scrl = 0;
    v0 = para_un;
    taiiku_kari_phase = 0;
    if (v0 >= 160) {
        taiiku_kari_gain = 640;
        taiiku_kari_vmax = 0x00020000;
    } else if (v0 >= 125) {
        taiiku_kari_gain = 544;
        taiiku_kari_vmax = 0x0001E000;
    } else if (v0 >= 100) {
        taiiku_kari_gain = 448;
        taiiku_kari_vmax = 0x0001C000;
    } else if (v0 >= 75) {
        taiiku_kari_gain = 352;
        taiiku_kari_vmax = 0x0001A000;
    } else {
        taiiku_kari_gain = 256;
        taiiku_kari_vmax = 0x00018000;
    }

    srand(vblnk_counter);

    for (i = 0; i < 2; i++) {
        taiiku_kari_hop[i] = 255;
    }

    taiiku_kari_hop_tmr[0] = 0;
    taiiku_kari_hop_tmr[1] = 60;
    taiiku_konami_mode = 255;
    taiiku_kari_girl = 255;
}
