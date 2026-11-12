/* Size: 316 bytes */
/* @SymbolSize: 316 */
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
    u16 h18;
    u16 h1A;
    s32 w1C;
    s32 w20;
} Rec;

u8 taiiku_ninin_rank[4] = {0};
s32 taiiku_ninin_tempo = 0;
s32 taiiku_ninin_gain = 0;
s32 taiiku_ninin_vmax = 0;
s32 taiiku_ninin_bonus[3] = {0};
Rec taiiku_ninin_run[4] = {0};

extern u8 taiiku_ninin_tick;

void taiiku_race_count_inc(void)
{
    s32 i;

    for (i = 0; i < 4; i++) {
        taiiku_ninin_run[i].w4 += 1;
        taiiku_ninin_run[i].w10 += 1;
        taiiku_ninin_run[i].h1A += 1;
        taiiku_ninin_run[i].h18 += 1;
    }
    taiiku_ninin_tick += 1;
}
