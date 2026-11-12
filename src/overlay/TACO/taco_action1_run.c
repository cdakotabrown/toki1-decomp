/* @SymbolSize: 552 */
/* @PostProc: scheduler_normalizer --const-fp --store-fp --branch-fp */
/* @PostProc: patch_insn_reg_field 0x150:rt:t1:t2 0x154:rs:t1:t2 0x154:rt:t1:t2 0x15c:rs:t1:t2 0x15c:rt:t2:t3 0x164:rs:t2:t3 0x16c:rs:t1:t2 0x16c:rt:t3:t1 0x174:rs:t3:t1 0x178:rs:t1:t2 0x17c:rt:t4:t5 0x180:rt:t4:t5 0x184:rs:t1:t2 0x1b4:rt:t5:t4 0x1b8:rt:t6:t4 0x1bc:rt:t5:t6 0x204:rt:t9:t7 0x208:rt:t7:t9 */
/* Address: 0x8014DA3C */
/* Size: 552 bytes (138 insns) */
#include "types.h"

extern u8 *taco_actor_rec;
extern void taco_rec_move(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i);
extern void taco_angles_set(s32 a, s32 b);
extern void taco_step_timer(void *a, void *b, s32 c, s32 d, s32 e);
void taco_action1_run(s32 sel)
{
  u8 *ptr;
  u8 *ptrB;
  if (sel < 15)
  {
    taco_rec_move(0, 0, -30, -30, -1, 0, 0, 50, 50);
    return;
  }
  if ((sel >= 16) && (sel < 25))
  {
    taco_rec_move(0, 0, -30, -30, 0, 0, 0, 50, 50);
    return;
  }
  if (sel == 25)
  {
    ptr = taco_actor_rec;
    if (ptr[2582] == 0)
    {
      ptr[2582] = 1;
      taco_angles_set(9, 3);
    }
    ptrB = taco_actor_rec;
    if (ptrB[2718] == 0)
    {
      ptrB[2718] = 1;
      taco_angles_set(13, 0);
    }
    return;
  }
  if ((sel >= 26) && (sel < 125))
  {
    u8 *ptr2;
    ptr = taco_actor_rec;
    if ((ptr[2582] != 0) && (ptr[2314] == 1))
    {
      taco_step_timer(&ptr[2451], &ptr[2450], 9, 3, 1);
    }
    ptr2 = taco_actor_rec;
    if ((ptr2[2718] != 0) && (ptr2[2314] == 1))
    {
      taco_step_timer(&ptr2[2587], &ptr2[2586], 13, 0, 1);
    }
    return;
  }
  if ((sel >= 125) && (sel < 135))
  {
    taco_rec_move(0, 0, 30, 30, 0, 0, 0, 50, 50);
    return;
  }
  if ((sel >= 135) && (sel < 150))
  {
    taco_rec_move(0, 0, 30, 30, 1, 0, 0, 50, 50);
  }
}
