/* @CompileFlags: -cckr */
#include "types.h"

typedef struct {
      u8  unk00;
      u8  unk01;
      u8  pad02[14];
      s16 unk10;
      s16 unk12;
      s16 unk14;
      s16 unk16;
      u8  pad18[32];
      u8  unk38;
      u8  unk39;
      u8  unk3A;
      u8  unk3B;
      s16 unk3C;
      s16 unk3E;
      s16 unk40;
      s16 unk42;
      s16 unk44;
      u8  unk46;
      u8  unk47;
      void *unk48;
      u8  unk4C;
      u8  unk4D;
      u8  unk4E;
      u8  unk4F;
      u8  unk50;
      u8  pad51[19];
} TTRec;

typedef struct {
      s16 unk0;
      s16 unk2;
      s16 unk4;
      s16 unk6;
      u8  pad8[8];
} TTCel;

typedef struct { u8 pad[0x2E]; u16 unk2E; } TTHit;
extern TTHit *tt_step_wk;
extern TTRec *tt_anim_buf;
extern TTCel *tt_scale_buf;

void tt_board_scroll(void)
{
    TTHit *hit;
    TTRec *p;
    TTCel *q;
    s32 i;

    hit = tt_step_wk;
    for (i = 0, p = tt_anim_buf + 64, q = tt_scale_buf; i < 25; i++) {
        q->unk4 = ((i / 5) * 72 - hit->unk2E - 92);
        q->unk6 = (hit->unk2E + (i % 5) * 72 - 72);
        p->unk14 = (q->unk4 - 36);
        p->unk16 = (q->unk6 - 36);
        p++;
        q++;
    }
    hit->unk2E++;
    if (hit->unk2E >= 72) {
        hit->unk2E = 0;
    }
}
