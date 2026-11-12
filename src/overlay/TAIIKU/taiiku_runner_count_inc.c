/* Size: 312 bytes */
/* @SymbolSize: 312 */
#include "types.h"

typedef struct {
      u8  pad0[7];
      u8  unk07;
      s32 pad08;
      u16 unk0C;
      u16 unk0E;
      s32 pad10;
      s32 unk14;
      s32 pad18;
      s32 unk1C;
      s32 pad20;
} Rec36;

static u8 pad_93f0[8] = {0};
Rec36 taiiku_kari_run[4] = {0};

void taiiku_runner_count_inc(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        taiiku_kari_run[i].unk0C += 1;
        taiiku_kari_run[i].unk0E += 1;
        taiiku_kari_run[i].unk14 += 1;
        taiiku_kari_run[i].unk1C += 1;
    }
    taiiku_kari_run[0].unk07 += 1;
}
