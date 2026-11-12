/* Address: 0x80148458 */
/* Size: 180 bytes (45 insns) */
/* @SymbolSize: 180 */
/* @CompileEnv: IDO52_LDA_NOINREG_CLASSES=4 */
#include "types.h"

typedef struct {
      s16 unk00;
      s16 unk02;
      s16 unk04;
      s16 unk06;
      s32 unk08;
      u8  unk0C;
      u8  unk0D;
      s16 unk0E;
      s32 unk10;
      s16 unk14;
      u16 unk16;
      s32 unk18;
} Racer;

typedef struct {
      u8  unk00;
      u8  pad01[3];
      s32 unk04;
      s32 unk08;
      s32 unk0C;
      s32 unk10;
      s32 unk14;
      s32 unk18;
} Trail;

extern Racer taiiku_spoon_bal[];
extern Trail taiiku_spoon_cpu[];

struct { u8 unk0; u8 pad[3]; s32 unk4; s32 unk8; } taiiku_spoon_pl = {0};
extern s32   taiiku_spoon_pl_w0C;
extern s32   taiiku_spoon_pl_spd;

void taiiku_racer_reset(s32 idx) {
    Racer *p;
    Trail *q;

    p = &taiiku_spoon_bal[idx];
    if (p->unk0C == 0) {
        return;
    }

    if ((u32)p->unk0D < 101) {
        return;
    }

    p->unk0C = 0;
    if (p == &taiiku_spoon_bal[0]) {
        taiiku_spoon_pl.unk0 = 0;
        taiiku_spoon_pl.unk8 = 0;
        taiiku_spoon_pl_w0C = 0;
        taiiku_spoon_pl.unk4 = 0;
        taiiku_spoon_pl_spd = 0;
    } else {
        q = &taiiku_spoon_cpu[idx - 1];
        q->unk00 = 0;
        q->unk08 = 0;
        q->unk0C = 0;
        q->unk04 = 0;
        q->unk14 = 0;
    }

    p->unk02 = 55;
    p->unk06 = 25;
    p->unk08 = 0;
    p->unk14 = 0;
    p->unk16 = 0;
    p->unk0D = 0;
}
