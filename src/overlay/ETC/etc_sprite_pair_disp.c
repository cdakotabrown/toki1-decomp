/* @PostProc: scheduler_normalizer --block-aware */
/* @SymbolSize: 500 */
/* Address: 0x8013DE90 */
/* Size: 500 bytes (125 instructions) */
#include "types.h"

extern s32 prim_alloc();
extern void goto_tpage();
extern void AddPrim();
extern void SetSemiTrans();
extern void SetShadeTex();
extern void SetSprt();
extern volatile s32 activeBuff;
extern s16 disp_ofs_x[];
extern s16 disp_ofs_y[];
extern s16 screen_w[];
extern u8 ot_buff0[];
typedef struct
{
  s32 unk0;
  u8 unk4;
  u8 unk5;
  u8 unk6;
  s8 pad7;
  s16 unk8;
  s16 unkA;
  s8 unkC;
  s8 unkD;
  s8 padE;
  s8 padF;
  s16 unk10;
  s16 unk12;
} Obj;
void etc_sprite_pair_disp(s32 a0)
{
  Obj *o;
  o = (Obj *) prim_alloc(20, activeBuff);
  SetSprt(o);
  SetSemiTrans(o, 0);
  SetShadeTex(o, 0);
  o->unk4 = 128;
  o->unk5 = 128;
  o->unk6 = 128;
  if (a0 == 1)
  {
    o->unk8 = disp_ofs_x[activeBuff] - 160;
    o->unkA = disp_ofs_y[activeBuff] - 101;
    o->unkD = 0;
    o->unkC = 0;
    o->unk10 = 192;
    o->unk12 = 160;
  }
  AddPrim(&ot_buff0[(activeBuff * 1024) + 52], o);
  goto_tpage(320, 256, 13, 0, 2);
  o = (Obj *) prim_alloc(20, activeBuff);
  SetSprt(o);
  SetSemiTrans(o, 0);
  SetShadeTex(o, 0);
  o->unk4 = 128;
  o->unk5 = 128;
  o->unk6 = 128;
  if (a0 == 1)
  {
    o->unk8 = screen_w[activeBuff - -10] + 32;
    o->unkA = screen_w[activeBuff + 12] - 101;
    o->unkD = 0;
    o->unkC = 0;
 do { o->unk10 = 128; o->unk12 = 160; } while (0);
  }
  AddPrim(&ot_buff0[(activeBuff * 1024) + 52], o);
  goto_tpage(512, 256, 13, 0, 2);
}
