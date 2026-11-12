/* @SymbolStart: 0x80143750 */
/* @SymbolSize: 624 */
#include "types.h"

typedef struct {
    u8 b0;
    u8 b1;
    u8 b2;
    u8 pad3;
    s32 w4;
    s32 w8;
    s32 wC;
    s32 w10;
    s16 h14;
    s16 h16;
    s16 h18;
    s16 h1A;
    s32 w1C;
    s32 w20;
} Rec;

u8 taiiku_ninin_rank[4] = {0};
s32 taiiku_ninin_tempo = 0;
s32 taiiku_ninin_gain = 0;
s32 taiiku_ninin_vmax = 0;
s32 taiiku_ninin_bonus[3] = {0};
Rec taiiku_ninin_run[4] = {0};
s32 taiiku_ninin_scrl = 0;
s32 taiiku_ninin_scrv = 0;

extern u8 taiiku_ninin_tick;
extern u8 taiiku_ninin_girl1;
extern s32 vblnk_counter;
extern s16 para_un;

extern void srand(s32 a0);

void taiiku_race_init3(void)
{
    s32 i;
    s32 level;
    s32 val;

    for (i = 0; i < 4; i++) {
        taiiku_ninin_rank[i] = 0;
        taiiku_ninin_run[i].b1 = 0;
        taiiku_ninin_run[i].b0 = 0;
        taiiku_ninin_run[i].w4 = 0;
        taiiku_ninin_run[i].w8 = 0;
        taiiku_ninin_run[i].wC = 0;
        taiiku_ninin_run[i].w10 = 0;
        taiiku_ninin_run[i].h14 = 0;
        taiiku_ninin_run[i].h16 = i * 8;
        taiiku_ninin_run[i].w1C = 1;
        taiiku_ninin_run[i].w20 = 0;
        taiiku_ninin_run[i].h1A = 0;
        taiiku_ninin_run[i].h18 = 0;
        taiiku_ninin_run[i].b2 = 0;
    }
    taiiku_ninin_run[0].h16 = -160;
    taiiku_ninin_scrv = 0;
    taiiku_ninin_scrl = 0;
    taiiku_ninin_tick = 0;
    level = para_un;
    taiiku_ninin_bonus[0] = 0x7E0000;
    taiiku_ninin_bonus[1] = 0x7F0000;
    taiiku_ninin_bonus[2] = 0x7D0000;

    if (level >= 180) {
        taiiku_ninin_tempo = 6;
        val = 0x00020000;
    } else if (level >= 100) {
        taiiku_ninin_tempo = 7;
        val = 0x0001E000;
    } else if (level >= 65) {
        taiiku_ninin_tempo = 8;
        val = 0x0001C000;
    } else if (level >= 30) {
        taiiku_ninin_tempo = 9;
        val = 0x0001A000;
    } else {
        taiiku_ninin_tempo = 10;
        val = 0x00018000;
    }

    if (taiiku_ninin_girl1 != 0) {
        val -= 8192;
    }

    taiiku_ninin_gain = val >> 3;
    taiiku_ninin_vmax = val;
    srand(vblnk_counter);
}
