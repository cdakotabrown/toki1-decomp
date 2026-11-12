/* @SymbolStart: 0x80137E24 */
/* @PostProc: patch_insn_reg_field 0x40:rt:t6:a0 0x44:rs:t6:a0 0x44:rt:t6:a0 0x4c:rs:t6:a0 0x60:rt:t7:t6 0x64:rs:t7:t6 0x64:rt:t7:t6 0x68:rt:t9:v1 0x6c:rs:t9:v1 0x6c:rt:t9:v1 0x74:rs:t7:t6 0x74:rt:t8:t7 0x78:rt:t8:t7 0x80:rs:t9:v1 0x80:rt:t0:t8 0x88:rt:t0:t8 0xac:rt:t1:t9 0xb0:rs:t1:t9 0xb4:rt:t2:v1 0xb8:rs:t2:v1 0xb8:rt:t2:v1 0xc0:rs:t2:v1 0xc0:rt:t3:t0 0xc8:rt:t3:t0 0xd4:rt:t4:t1 0xd8:rs:t4:t1 0xec:rt:t5:t2 0xf0:rs:t5:t2 0xf0:rt:t5:t2 0xf8:rs:t5:t2 0xf8:rt:t6:t3 0xfc:rs:t6:t3 0x100:rt:t7:t4 0x104:rs:t7:t4 0x104:rt:t7:t4 0x10c:rs:t7:t4 0x10c:rt:t8:t5 0x110:rt:t8:t5 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x28:rs:v0:v1 0x30:rs:v0:v1 */
/* @SymbolSize: 276 */
/* Address: 0x80137E2C */
/* Size: 268 bytes (67 insns) */
#include "types.h"

extern s16 taco_time_left;
extern u8 step_mode;
extern s32 frame_counter;
extern s16 mes_win_cw_y;
extern void taco_xa_sector_set(void);
extern void sub_sub_step_up(void);
extern void taco_mes_win_set(s32);
extern s32 get_CD_State(void);
extern void dec_bg_show(s32);
void taco_main7_main2(void)
{
    u8 step = step_mode;

  switch (step)
  {
    case 0:
      if (taco_time_left < 10)
    {
      taco_xa_sector_set();
      taco_time_left = (s16) (taco_time_left + 1);
      step_mode = (u8) (((u8) step_mode) + 1);
    }
    else
    {
      sub_sub_step_up();
      taco_mes_win_set(2);
    }
      break;

    case 1:
      if ((get_CD_State() & 4) != 0)
    {
      step_mode = (u8) (((u8) step_mode) + 1);
    }
      break;

    case 2:
      if ((get_CD_State() & 2) != 0)
    {
      step_mode = 0;
    }
      break;

  }

  dec_bg_show(2);

  if ((frame_counter & 3) == 3)
  {
    mes_win_cw_y = (s16) (mes_win_cw_y - 1);
  }
}
