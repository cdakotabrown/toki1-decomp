/* @PostProc: swap_instructions --offset1 0xb4 --offset2 0xb8 */
/* Address: 0x8013F784 */
#include "types.h"

typedef struct Sprite {
      u32 unk0;
      u8 unk4;
      u8 unk5;
      u8 unk6;
      u8 unk7;
      s16 unk8;
      s16 unkA;
      u8 unkC;
      u8 unkD;
      s16 unkE;
      s16 unk10;
      s16 unk12;
} Sprite;

extern s32 activeBuff;
extern s16 screen_w[];
extern volatile s32 frame_counter;
extern u8 ot_buff0[];

extern Sprite *prim_alloc(s32, s32);
extern void SetSprt(Sprite *);
extern void SetSemiTrans(Sprite *, s32);
extern s16 GetClut(s32, s32);
extern void AddPrim(u8 *, Sprite *);
extern void dtd_on_tpage(s32, s32, s32, s32, s32);

void
taco_icon_disp(s32 arg0, s32 arg1, s32 arg2)
{
    Sprite *sp;
    s32 v;

    sp = prim_alloc(20, activeBuff);
    SetSprt(sp);
    sp->unk10 = 16;
    sp->unk12 = 16;
    SetSemiTrans(sp, 1);
    sp->unk4 = 128;
    sp->unk5 = 128;
    sp->unk6 = 128;
    sp->unk8 = (s16)(screen_w[activeBuff + 10] + arg0);
    sp->unkA = (s16)(screen_w[activeBuff + 12] + arg1);
    sp->unkC = 192;
    sp->unkD = (u8)(arg2 * 16 - 48);
    v = (s32)((((u32)frame_counter >> 1) % 14U) << 4);
    if (arg2 == 3) {
        sp->unkE = GetClut(v, 490);
    } else {
        sp->unkE = GetClut(v, 489);
    }
    AddPrim(&ot_buff0[activeBuff * 1024 + 4], sp);
    dtd_on_tpage(896, 256, 1, 0, 0);
}
