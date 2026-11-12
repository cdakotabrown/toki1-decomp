/* @SymbolSize: 204 */
/* @PostProc: scheduler_normalizer */
/* @PostProc: patch_insn_reg_field 0xb4:rt:t8:t6 0xbc:rd:t2:t7 0xbc:rt:t8:t6 0xc0:rs:t2:t7 0xc0:rt:t4:t8 0xc8:rt:t4:t8 */
/* Address: 0x801489BC */
/* Size: 204 bytes (51 insns) */
#include "types.h"
#include "tokimeki/class_w.h"

extern u8 taiiku_spoon_rank[4];
extern s16 cal_cw_frame;
extern s16 cw97_frame;
extern s16 cw98_frame;
extern CLASS_W class_w[];
extern s32 Vblnk_Timer(void);
extern void sub_sub_step_up(void);
void taiiku_spoon_result_frame_step(void)
{
  s32 i;
  s32 count;
  for (i = 0; i < 4; i++)
  {
    if (taiiku_spoon_rank[i] == 1)
    {
      count = i;
    }
  }

  (void) (&count);
  i = 0;
  if (((u32) Vblnk_Timer()) >= 51u)
  {
    cal_cw_frame += 1;
    cw97_frame += 2;
    cw98_frame += 3;
    class_w[99].frame += 4;
    i = 2;
    class_w[104].frame = ((((((((((class_w[104].frame & 0xFFFFFFFFFFFFFFFFu) & 0xFFFFFFFFFFFFFFFFu) & 0xFFFFFFFFFFFFFFFFu) & 0xFFFFFFFFFFFFFFFFu) & 0xFFFFFFFFFFFFFFFFu) & 0xFFFFFFFFFFFFFFFFu) & 0xFFFFFFFFFFFFFFFFu) & 0xFFFFFFFFFFFFFFFFu) & 0xFFFFFFFFFFFFFFFFu) + count) + i;
    sub_sub_step_up();
  }
}
