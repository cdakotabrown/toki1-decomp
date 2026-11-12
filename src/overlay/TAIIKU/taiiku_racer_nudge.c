/* Address: 0x80147C08 */
/* Size: 168 bytes (42 insns) */
/* @SymbolSize: 168 */
/* @CompileEnv: IDO52_NO_CSE=1 IDO52_LDA_NOINREG_CLASSES=4 */
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

extern Racer taiiku_spoon_bal[];
extern s32 taiiku_spoon_pl_acc;
extern s32 taiiku_spoon_scrv;

void taiiku_racer_nudge(s32 idx) {
    Racer *p;
    s32 v;

    p = &taiiku_spoon_bal[idx];
    if (p->unk0C != 0) {
        return;
    }

    if (p == &taiiku_spoon_bal[0]) {
        if (taiiku_spoon_pl_acc <= 0) {
            if (taiiku_spoon_scrv == 0) {
                return;
            }
        }
    }

    if ((u32)p->unk16 < 6) {
        return;
    }

    v = p->unk08;
    if (v > 0) {
        p->unk02 = p->unk02 + 1;
        v = p->unk08;
        p->unk16 = 0;
    }

    if (v >= 0) {
        return;
    }

    p->unk02 = p->unk02 - 1;
    p->unk16 = 0;
}
