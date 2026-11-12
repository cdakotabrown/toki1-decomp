/* Address: 0x80144E1C */
/* Size: 484 bytes (121 insns) */
/* @PostProc: patch_stack_complete --old 88 --new 112 */
/* @PostProc: remap_stack_slots --map '80:100' */
#include "types.h"

typedef struct {
    u8  pad00[24];
    s16 f24;
    s16 f26;
    s16 f28;
    u8  pad30[12];
    s16 f42;
    u8  pad44[24];
} Rec68;

typedef struct {
    u8  f00;
    u8  pad01;
    u8  f02;
    u8  pad03;
    s32 f04;
    u8  pad08[4];
    s32 f12;
    u8  pad16[8];
    s16 f24;
    u16 f26;
    s32 f28;
    u8  pad32[4];
} Rec36;

typedef struct {
    s16 v[3];
} Buf6;

extern Rec68 class_w[];
extern Rec36 taiiku_ninin_run[];
extern Buf6 taiiku_race_stumble_tbl;

extern s32 rand(void);

void taiiku_race_stumble(void)
{
    Buf6 thr;
    s32 i;

    thr = taiiku_race_stumble_tbl;
    for (i = 0; i < 3; i++) {
        if (i + 54 != class_w[97 + i].f24 &&
            taiiku_ninin_run[i + 1].f04 == 0 &&
            (taiiku_ninin_run[i + 1].f00 == 2U || taiiku_ninin_run[i + 1].f00 == 6) &&
            taiiku_ninin_run[i + 1].f28 == 1 &&
            thr.v[i] < (s32)(rand() & 0xFF) &&
            taiiku_ninin_run[i + 1].f02 == 1U) {
            taiiku_ninin_run[i + 1].f28 = 0;
            taiiku_ninin_run[i + 1].f26 = 0;
            taiiku_ninin_run[i + 1].f12 = 0;
            class_w[97 + i].f24 =
                (s16)(rand() % 3 + i * 9 + 24);
        }
    }
    for (i = 0; i < 3; i++) {
        if (i + 54 != class_w[97 + i].f24 &&
            taiiku_ninin_run[i + 1].f28 == 0 &&
            taiiku_ninin_run[i + 1].f26 >= 30U) {
            taiiku_ninin_run[i + 1].f28 = 1;
            taiiku_ninin_run[i + 1].f24 = 0;
            taiiku_ninin_run[i + 1].f00 = 0;
            taiiku_ninin_run[i + 1].f02 = 0;
            class_w[97 + i].f24 = (s16)(i + 54);
        }
    }
}
